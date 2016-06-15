#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "zillow/Database.hpp"
#include "zillow/Serialization.hpp"
#include "zillow/Serialization.hpp"
#include "zillow/XMLParser.hpp"

void parseDeepSearchResults() {
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults =
        doc.load_file("deepSearchResults.xml");

    // assert(parseResults == pugi::status_ok);

    auto request = zillow::parseDeepSearchResultsRequest(
        doc.child("SearchResults:searchresults").child("request"));
    fmt::print("request: \n\taddress: {0}\n\tcitystatezip : {1}\n",
               std::get<0>(request), std::get<1>(request));

    auto message = zillow::parseMessage(
        doc.child("SearchResults:searchresults").child("message"));
    fmt::print("message: \n\ttext: {0}\n\tcode : {1}\n", std::get<0>(message),
               std::get<1>(message));

    auto response = zillow::parseDeepSearchResultsResponse(
        doc.child("SearchResults:searchresults")
            .child("response")
            .child("results")
            .child("result"));

    std::ostringstream os;
    zillow::print<cereal::JSONOutputArchive>(os, response);
    fmt::print("{}\n", os.str());

    std::vector<decltype(response)> data{response, response};

    {
        std::ostringstream os;
        zillow::print<cereal::JSONOutputArchive>(os, response);
        zillow::print<cereal::XMLOutputArchive>(os, response);
        fmt::print("{}\n", os.str());
    }
}

void parseDeepCompsResults() {
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults = doc.load_file("deepCompsResults.xml");
    // assert(parseResults == pugi::status_ok);
    auto request = zillow::parseDeepCompsRequest(
        doc.child("Comps:comps").child("request"));
    fmt::print("request: \n\tzpid: {0}\n\tcount : {1}\n", std::get<0>(request),
               std::get<1>(request));

    auto message =
        zillow::parseMessage(doc.child("Comps:comps").child("message"));
    fmt::print("message: \n\ttext: {0}\n\tcode : {1}\n", std::get<0>(message),
               std::get<1>(message));

    // zillow::dfs(rootNode.child("response").child("properties"), "");
    auto results = zillow::parseDeepCompsResponse(
        doc.child("Comps:comps").child("response").child("properties"));

    // const zillow::DeepSearchResults &principal = std::get<0>(results);
    auto const &deepComps = std::get<1>(results);
    auto const &edges = std::get<2>(results);

    fmt::print("Number of comps element: {}\n", deepComps.size());
    for (auto const &item : deepComps) {
        std::ostringstream os;
        zillow::print<cereal::JSONOutputArchive>(os, item);
        fmt::print("{}\n", os.str());
    }

    {
        std::ostringstream os;
        zillow::print<cereal::JSONOutputArchive>(os, deepComps);
        fmt::print("{}\n", os.str());
        // cereal::JSONOutputArchive oar(os);
        // oar(deepComps);
    }

    {
        std::ostringstream os;
        zillow::print<cereal::JSONOutputArchive>(os, edges);
        fmt::print("{}\n", os.str());
    }

    // Write information to the database
    zillow::writeToSQLite("database.db", deepComps, edges);
}

int main() {

    // pugi::xpath_query query_name("/SearchResults:searchresults");
    // pugi::xpath_query query_address(
    //     "/SearchResults:searchresults/request/address");
    // pugi::xpath_query query_zipcode(
    //     "/SearchResults:searchresults/response/results/result/zpid");
    // std::cout << "Query address: " << query_address.evaluate_string(doc)
    //           << "\n";
    // std::cout << "Query zipcode: " << query_zipcode.evaluate_string(doc)
    //           << "\n";

    parseDeepSearchResults();
    parseDeepCompsResults();

    return 0;
}
