#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "zillow/Serialization.hpp"
#include "zillow/Serialization.hpp"
#include "zillow/XMLParser.hpp"

void parseDeepSearchResults() {
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults =
        doc.load_file("deepSearchResults.xml");

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
    zillow::print(response);

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

    auto request = zillow::parseDeepCompsRequest(
        doc.child("Comps:comps").child("request"));
    fmt::print("request: \n\tzpid: {0}\n\tcount : {1}\n", std::get<0>(request),
               std::get<1>(request));

    auto message =
        zillow::parseMessage(doc.child("Comps:comps").child("message"));
    fmt::print("message: \n\ttext: {0}\n\tcode : {1}\n", std::get<0>(message),
               std::get<1>(message));

    // zillow::dfs(rootNode.child("response").child("properties"), "");
    zillow::DeepSearchResults principal;
    std::vector<zillow::DeepSearchResults> housses;
    std::vector<zillow::EdgeData> edges;
    std::tie(principal, housses, edges) = zillow::parseDeepCompsResponse(
        doc.child("Comps:comps").child("response").child("properties"));

    for (auto const &item : housses) {
        fmt::print("================{}==============\n", item.zpid);
        zillow::print(item);
    }

    for (auto const &item : edges) {
        zillow::print(item);
    }
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
    // parseDeepCompsResults();

    return 0;
}
