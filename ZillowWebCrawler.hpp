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

#include "Database.hpp"
#include "Serialization.hpp"
#include "XMLParser.hpp"
#include "Zillow.hpp"

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
        std::string aStreet(info.Street);
        strrep(aStreet, ' ', '+');
        std::string aCity(info.City);
        strrep(aCity, ' ', '+');

        return "http://www.zillow.com/webservice/"
               "GetDeepSearchResults.htm?zws-id=" +
               zwpid + "&address=" + aStreet + "&citystatezip=" + aCity +
               "%2C+" + info.State;
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
        explicit Crawler(const std::string &userid, const size_t num)
            : Vertexes(), Edges(), Visited(), Processed(), Queue(), HouseIDs(),
              zwpid(userid), SQLiteDatabase("database.db"),
              NoSQLDatabase("info"), max_houses(num),
              NoSQLDatabaseWriter(NoSQLDatabase) {}

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
            // assert(parseResults == pugi::status_ok);
            auto message = zillow::parseMessage(
                doc.child("SearchResults:searchresults").child("message"));

            assert(message.Code == 0);
            if (message.Code) {
                fmt::print("Error ===> Could not query this address: {}\n",
                           queryCmd);
                std::ostringstream os;
                print<cereal::JSONOutputArchive>(os, message);
                fmt::print("{}\n", os.str());
                return;
            }

            auto response = zillow::parseDeepSearchResultsResponse(
                doc.child("SearchResults:searchresults")
                    .child("response")
                    .child("results")
                    .child("result"));
            auto zpid = response.zpid;

            // For debuging purpose
            {
                std::ostringstream os;
                print<cereal::JSONOutputArchive>(os, response);
                fmt::print("{}\n", os.str());
            }

            Visited.insert(zpid);
            HouseIDs.insert(zpid);
            Vertexes.emplace_back(std::move(response));

            // Now crawl more data
            Queue.push_back(zpid);
            traverse(count);
        }

        void disp(const int verbose_level) {
            switch (verbose_level) {
              case 1:
                fmt::print("Number of vertexes: {}\n", Vertexes.size());
                fmt::print("Number of edges: {}\n", Edges.size());
                break;
            }
        }

        void save() const {
            // Serialize all data to JSON
            std::stringstream output;
            {
                cereal::JSONOutputArchive oar(output);
                oar(cereal::make_nvp("Vertexes", Vertexes),
                    cereal::make_nvp("Edges", Edges));
            }
            writeTextFile(output, "crawled_data.json");

            // Write to SQLite database
            fmt::print("Number of vertexes: {0}\n", Vertexes.size());
            fmt::print("Number of edges: {0}\n", Edges.size());
            writeToSQLite("database.db", Vertexes, Edges);
        }

        // Will need to use BFS traversal.
        void traverse(int count) {
            while (!Queue.empty() && (Vertexes.size() < max_houses)) {
                const unsigned long zpid = Queue.front();
                Queue.pop_front();

                // Debug message
                disp(1);
                fmt::print("zpid = {}\n", zpid);

                // Get DeepComps results
                std::string queryCmd =
                    zillow::generateDeepCompsQuery(zwpid, zpid, count);
                std::stringstream output;

                auto results = zillow::query(queryCmd, output);
                assert(results);

                const std::string aKey = "deepComps-" + std::to_string(zpid);
                NoSQLDatabaseWriter.write(aKey, output.str());
                
                // Parse XML data
                fmt::print("Parse XML data obtained from this query {}\n",
                           queryCmd);

                pugi::xml_document doc;
                pugi::xml_parse_result parseResults = doc.load(output);
                // assert(parseResults == pugi::status_ok);

                Message message = zillow::parseMessage(
                    doc.child("Comps:comps").child("message"));
                
                if (message.Code) {
                    fmt::print("Error ===> Could not query this address: {}\n",
                               queryCmd);
                    std::ostringstream os;
                    print<cereal::JSONOutputArchive>(os, message);
                    fmt::print("{}\n", os.str());
                    
                    // If we could not query data for a given address then skip it.
                    continue;
                }

                // fmt::print("Parse deepComps data\n");

                std::vector<DeepSearchResults> houses;
                std::vector<EdgeData> e;
                std::tie(houses, e) =
                    zillow::parseDeepCompsResponse(doc.child("Comps:comps")
                                                       .child("response")
                                                       .child("properties"));

                // fmt::print("Get the results\n");

                // Update vertex information
                Visited.insert(zpid);

                // TODO: Use move to improve the performance.
                for (auto &item : e) {
                    Edges.emplace_back(item);
                }

                // Update house information
                fmt::print("Update house information\n");
                for (auto const &aHouse : houses) {
                    auto child_zpid = aHouse.zpid;
                    auto aCity = aHouse.info.HouseAddress.City;
                    if (Visited.find(child_zpid) == Visited.end()) {
                        fmt::print("child_zpid = {0} -> {1}, {2} {3}\n",
                                   child_zpid, aHouse.info.HouseAddress.Street,
                                   aHouse.info.HouseAddress.City,
                                   aHouse.info.HouseAddress.State);
                        Queue.push_back(child_zpid);

                        // Only insert a current house if it does not exist in
                        // the database.
                        if (HouseIDs.find(child_zpid) == HouseIDs.end()) {
                            Vertexes.emplace_back(aHouse);
                            HouseIDs.insert(child_zpid);
                        }
                    }
                }
            }
        }

      private:
        std::vector<DeepSearchResults> Vertexes;
        std::vector<EdgeData> Edges;
        std::unordered_set<unsigned long> Visited;
        std::unordered_set<unsigned long> Processed;
        std::deque<unsigned long> Queue;
        std::unordered_set<IDType> HouseIDs;

        std::string zwpid;
        std::string SQLiteDatabase;
        std::string NoSQLDatabase;
        size_t max_houses;
        utils::Writer NoSQLDatabaseWriter;
    };
}
#endif
