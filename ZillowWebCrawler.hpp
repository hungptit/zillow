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
#include "Utils.hpp"
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
    bool doRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request,
                   Poco::Net::HTTPResponse &response, std::stringstream &output) {
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

    // Craw all related data for a given house using DFS algorithms.
    class Crawler {
      public:
        explicit Crawler(const std::string &userid, const size_t num)
            : MaxHouse(num), Vertexes(), Edges(), Visited(), Processed(), Queue(), HouseIDs(),
              zwpid(userid), Database("database"), NoSQLWriter(Database) {}

        explicit Crawler(const std::string &userid, const size_t num,
                         const std::string &dataFile)
            : MaxHouse(num), Vertexes(), Edges(), Visited(), Processed(), Queue(), HouseIDs(),
              zwpid(userid), Database(dataFile), NoSQLWriter(Database) {}

        template <typename Constraints>
        void exec(const Address &aHouse, const int count, Constraints &cons) {
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
            if (parseResults) {
                fmt::print("status: {}\n", parseResults.status);
                fmt::print("description: {}\n", parseResults.description());
            }
            // assert(parseResults == pugi::status_ok);
            auto message =
                zillow::parseMessage(doc.child("SearchResults:searchresults").child("message"));

            assert(message.Code == 0);
            if (message.Code) {
                fmt::print("Error ===> Could not query this address: {}\n", queryCmd);
                print<cereal::JSONOutputArchive>(message);
                return;
            }

            auto response =
                zillow::parseDeepSearchResultsResponse(doc.child("SearchResults:searchresults")
                                                           .child("response")
                                                           .child("results")
                                                           .child("result"));
            auto zpid = response.zpid;
            print<cereal::JSONOutputArchive>(response);

            Visited.insert(zpid);
            HouseIDs.insert(zpid);
            Vertexes.emplace_back(std::move(response));

            // Now crawl more data
            Queue.push_back(zpid);
            traverse(count, cons);
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
            // std::stringstream output;
            // {
            //     cereal::JSONOutputArchive oar(output);
            //     oar(cereal::make_nvp("Vertexes", Vertexes),
            //         cereal::make_nvp("Edges", Edges));
            // }
            // writeTextFile(output, "crawled_data.json");

            // Write to SQLite database
            fmt::print("Number of vertexes: {0}\n", Vertexes.size());
            fmt::print("Number of edges: {0}\n", Edges.size());
            writeToSQLite(Database + ".db", Vertexes, Edges);
        }

        // Will need to use BFS traversal.
        template <typename Constraints> void traverse(int count, Constraints &cons) {
            bool needToStop = false;
            while (!Queue.empty() && (Vertexes.size() < MaxHouse) && !needToStop) {
                const index_type zpid = Queue.front();
                Queue.pop_front();

                // Debug message
                disp(1);
                fmt::print("zpid = {}\n", zpid);

                // Get DeepComps results
                std::string queryCmd = zillow::generateDeepCompsQuery(zwpid, zpid, count);
                std::stringstream output;

                auto results = zillow::query(queryCmd, output);
                if (!results) {
                    fmt::print("Cannot execute this query command: {}\n", queryCmd);
                    continue;
                }

                // Write crawled data to NoSQL database
                const std::string aKey = "deepComps-" + std::to_string(zpid);
                NoSQLWriter.write(aKey, output.str());

                // Parse XML data
                fmt::print("Parse XML data obtained from this query {}\n", queryCmd);

                pugi::xml_document doc;
                pugi::xml_parse_result parseResults = doc.load(output);

                if (parseResults) {
                    fmt::print("status: {}\n", parseResults.status);
                    fmt::print("description: {}\n", parseResults.description());
                }

                Message message =
                    zillow::parseMessage(doc.child("Comps:comps").child("message"));

                if (message.Code) {
                    fmt::print("Error ===> Could not query this address: {}\n", queryCmd);
                    print<cereal::JSONOutputArchive>(message);
                    // If we could not query data for a given address then skip
                    // it.
                    if (message.Code == 7) {
                        // We have reach the maximum number of calls.
                        needToStop = true;
                    }
                    continue;
                }

                // fmt::print("Parse deepComps data\n");

                std::vector<DeepSearchResults> houses;
                std::vector<EdgeData> e;
                std::tie(houses, e) = zillow::parseDeepCompsResponse(
                    doc.child("Comps:comps").child("response").child("properties"));

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
                        auto const anAddress = aHouse.info.HouseAddress;
                        if (cons.isValid(anAddress)) {
                            fmt::print("child_zpid = {0} -> {1}, {2} {3}\n", child_zpid,
                                       anAddress.Street, anAddress.City, anAddress.State);
                            Queue.push_back(child_zpid);
                        }

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
        size_t MaxHouse;
        std::vector<DeepSearchResults> Vertexes;
        std::vector<EdgeData> Edges;
        std::unordered_set<index_type> Visited;
        std::unordered_set<index_type> Processed;
        std::deque<index_type> Queue;
        std::unordered_set<index_type> HouseIDs;
        std::string zwpid;
        std::string Database;
        utils::Writer NoSQLWriter;
    };
}
#endif
