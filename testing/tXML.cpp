#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "zillow/XMLParser.hpp"

void parse(pugi::xml_node &aNode, const std::string &prefix) {
    const std::string rootName = aNode.name();
    if (!rootName.compare("request")) {
        auto results = zillow::parseRequest(aNode);
        fmt::print("request: \n\taddress: {0}\n\tcitystatezip : {1}\n",
                   std::get<0>(results), std::get<1>(results));
        return;
    } else if (!rootName.compare("message")) {
        auto results = zillow::parseMessage(aNode);
        fmt::print("message: \n\ttext: {0}\n\tcode : {1}\n",
                   std::get<0>(results), std::get<1>(results));
        return;
    } else if (!rootName.compare("response")) {
        auto results = zillow::parseResponse(aNode);
        zillow::print(results);
        return;
    } else {
        for (pugi::xml_node aChild = aNode.first_child(); aChild;
             aChild = aChild.next_sibling()) {
            const std::string nodeName = aChild.name();

            if (aChild.type() == pugi::node_pcdata) {
                // Skip this node
            } else {
                parse(aChild, prefix + "/" + nodeName);
            }
        }
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

    // dfs(info, std::string("/") + std::string(info.name()));
    {
        pugi::xml_document doc;
        pugi::xml_parse_result parseResults =
            doc.load_file("deepSearchResults.xml");
        pugi::xml_node root = doc.child("SearchResults:searchresults");
        parse(root, std::string(""));
    }

    // {
    //     pugi::xml_document doc;
    //     pugi::xml_parse_result parseResults =
    //         doc.load_file("deepCompsResults.xml");
    //     pugi::xml_node info = doc.child("Comps:comps");
    //     DOMSearch results(info);
    //     results.print();
    // }

    return 0;
}
