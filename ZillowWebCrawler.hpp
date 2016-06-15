#ifndef ZillowWebCrawler_hpp_
#define ZillowWebCrawler_hpp_

#include "Poco/Exception.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include "XMLParser.hpp"
#include "Zillow.hpp"
#include "Database.hpp"
#include "utils/LevelDBIO.hpp"

#include <deque>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {
    bool doRequest(Poco::Net::HTTPClientSession &session,
                   Poco::Net::HTTPRequest &request,
                   Poco::Net::HTTPResponse &response,
                   std::stringstream &output) {
        session.sendRequest(request);
        std::istream &rs = session.receiveResponse(response);
        if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
            Poco::StreamCopier::copyStream(rs, output);
            return true;
        } else {
            // fmt::print("Cannot query this link: {}", path);
            fmt::print("Status: {0}\nReason: {1}\n", response.getStatus(),
                       response.getReason());
            return false;
        }
    }
}

namespace zillow {
    bool query(const std::string &aLink, std::stringstream &output) {
        Poco::URI uri(aLink);
        std::string path(uri.getPathAndQuery());
        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path,
                                       Poco::Net::HTTPMessage::HTTP_1_1);
        Poco::Net::HTTPResponse response;
        return doRequest(session, request, response, output);
    }

    void strrep(std::string &s, char oldChar, char newChar) {
        const size_t N = s.size();
        for (size_t idx = 0; idx < N; ++idx) {
            if (s[idx] == oldChar) {
                s[idx] = newChar;
            }
        }
    }

    std::string generateDeepSearchQuery(const std::string &zwpid,
                                        const Address &info) {
        std::string aStreet(std::get<STREET>(info));
        strrep(aStreet, ' ', '+');
        std::string aCity(std::get<CITY>(info));
        strrep(aCity, ' ', '+');

        return "http://www.zillow.com/webservice/"
               "GetDeepSearchResults.htm?zws-id=" +
               zwpid + "&address=" + aStreet + "&citystatezip=" + aCity +
               "%2C+" + std::get<STATE>(info);
    }

    std::string generateDeepCompsQuery(const std::string &zwpid,
                                       const long zpid, const int count) {
        return "http://www.zillow.com/webservice/GetDeepComps.htm?zws-id=" +
               zwpid + "&zpid=" + std::to_string(zpid) + "&count=" +
               std::to_string(count);
    }
  
  
    // Craw all related data for a given house using DFS algorithms.
    class Crawler {
      public:
        explicit Crawler(const std::string &userid)
            : vertexes(), edges(), zwpid(userid),
              DeepSearchWriter(deepSearchDatabaseFile),
              DeepCompsWriter(deepCompsDatabaseFile),
              UpdatedPropertyDetailsWriter(updatePropertyDetailsDatabaseFile),
              Visited(), Processed() {}

        void exec(const Address &aHouse, const int count) {
            // Crawl the data of a given house
            std::string queryCmd = generateDeepSearchQuery(zwpid, aHouse);
            std::stringstream output;
            bool status = zillow::query(queryCmd, output);
            if (!status) {
                fmt::print("Cannot query this link: {}", queryCmd);
                return;
            }

            pugi::xml_document doc;
            pugi::xml_parse_result parseResults = doc.load(output);
            assert(parseResults == pugi::status_ok);
            auto message = zillow::parseMessage(
                doc.child("SearchResults:searchresults").child("message"));

            if (std::get<1>(message)) {
                fmt::print("Could not query this link: {0}\n", queryCmd);
                return;
            }

            auto response = zillow::parseDeepSearchResultsResponse(
                doc.child("SearchResults:searchresults")
                    .child("response")
                    .child("results")
                    .child("result"));
            auto zpid = response.zpid;

            Visited.insert(zpid);

            // Update vertexes
            vertexes.emplace_back(std::move(response));

            // Now crawl more data
            Queue.push_back(zpid);
            traverse(count);
        }

        void save() const {
            // std::set<DeepSearchResults> results(vertexes.begin(),
            // vertexes.end());
            // Save to SQlite3 database.
          writeToSQLite("database.db", vertexes, edges);
          
            //
        }

        // Will need to use BFS traversal.
        void traverse(int count) {
            while (!Queue.empty() && (vertexes.size() < max_housses)) {
                unsigned long zpid = Queue.front();
                Queue.pop_front();

                fmt::print("zpid = {}\n", zpid);

                // Get DeepComps results
                std::string queryCmd =
                    zillow::generateDeepCompsQuery(zwpid, zpid, count);
                std::stringstream output;

                auto results = zillow::query(queryCmd, output);
                assert(results);

                writeTextFile(output,
                              "deepComps_" + std::to_string(zpid) + ".xml");

                // Parse XML data
                pugi::xml_document doc;
                pugi::xml_parse_result parseResults = doc.load(output);
                assert(parseResults == pugi::status_ok);
                auto message = zillow::parseMessage(
                    doc.child("Comps:comps").child("message"));

                if (std::get<1>(message)) {
                    fmt::print("Error ===> Could not query this address: {}\n",
                               queryCmd);
                    return;
                }

                DeepSearchResults principal;
                std::vector<zillow::DeepSearchResults> housses;
                std::vector<zillow::EdgeData> e;
                std::tie(principal, housses, e) =
                    zillow::parseDeepCompsResponse(doc.child("Comps:comps")
                                                       .child("response")
                                                       .child("properties"));

                // Update vertex information
                Visited.insert(zpid);

                // Update edge information
                std::move(e.begin(), e.end(), edges.end());

                for (auto const &aHouse : housses) {
                    auto child_zpid = aHouse.zpid;
                    auto aCity = std::get<CITY>(aHouse.address);
                    if (Visited.find(child_zpid) == Visited.end()) {
                        fmt::print("child_zpid = {0} -> {1}, {2} {3}\n",
                                   child_zpid, std::get<STREET>(aHouse.address),
                                   std::get<CITY>(aHouse.address),
                                   std::get<STATE>(aHouse.address));
                        Queue.push_back(child_zpid);
                        vertexes.emplace_back(aHouse);
                    }
                }
            }
        }

      private:
        std::vector<DeepSearchResults> vertexes;
        std::vector<EdgeData> edges;
        std::string zwpid;
        const std::string deepSearchDatabaseFile = "deep_search";
        const std::string deepCompsDatabaseFile = "deep_comps";
        const std::string updatePropertyDetailsDatabaseFile =
            "updated_property_detail";
        const size_t max_housses = 100;
        utils::Writer DeepSearchWriter;
        utils::Writer DeepCompsWriter;
        utils::Writer UpdatedPropertyDetailsWriter;

        std::unordered_set<unsigned long> Visited;
        std::unordered_set<unsigned long> Processed;
        std::deque<unsigned long> Queue;
    };
}
#endif
