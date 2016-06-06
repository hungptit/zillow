#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

class DeepSearchResults {
  public:
    DeepSearchResults(pugi::xml_node &info) { traverse(info, info.name()); }

    void print() const {
        for (auto const &item : Nodes) {
            fmt::print("{0} : {1}\n", std::get<0>(item), std::get<1>(item));
        }

        for (auto const &item : Attributes) {
            fmt::print(" -> {0} : {1}\n", std::get<0>(item), std::get<1>(item));
        }
    }

  private:
    void traverse(pugi::xml_node &aNode, const std::string &prefix) {
        for (pugi::xml_node aChild = aNode.first_child(); aChild;
             aChild = aChild.next_sibling()) {
            const std::string nodeName = aChild.name();

            if (aChild.type() == pugi::node_pcdata) {
                std::string nodePath = prefix + "/" + nodeName;
                Nodes.emplace_back(std::make_tuple(nodePath, aChild.value()));
            } else {
                traverse(aChild, prefix + "/" + nodeName);
            }
        }

        // Get attributes for this node.
        for (pugi::xml_attribute anAttribute = aNode.first_attribute();
             anAttribute; anAttribute = anAttribute.next_attribute()) {
            Attributes.emplace_back(std::make_tuple(
                prefix + "/" + anAttribute.name(), anAttribute.value()));
        }
    }

    // Data
    std::vector<std::tuple<std::string, std::string>> Nodes;
    std::vector<std::tuple<std::string, std::string>> Attributes;
};

void dfs(pugi::xml_node aNode, const std::string &prefix) {
    for (pugi::xml_node aChild = aNode.first_child(); aChild;
         aChild = aChild.next_sibling()) {
        const std::string nodeName = aChild.name();
        if (aChild.type() == pugi::node_pcdata) {
            std::cout << "Type" << aChild.type() << " "
                      << prefix + "/ --> " + nodeName << ":" << aChild.value()
                      << "\n";
        }

        for (pugi::xml_attribute anAttribute = aNode.first_attribute();
             anAttribute; anAttribute = anAttribute.next_attribute()) {
            std::cout << "Attribute -> " << anAttribute.name() << ":"
                      << anAttribute.value() << "\n";
        }
        dfs(aChild, prefix + "/" + nodeName);
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
        pugi::xml_node info = doc.child("SearchResults:searchresults");
        DeepSearchResults results(info);
        results.print();
    }

    {
        pugi::xml_document doc;
        pugi::xml_parse_result parseResults =
            doc.load_file("deepCompsResults.xml");
        pugi::xml_node info = doc.child("Comps:comps");
        DeepSearchResults results(info);
        results.print();
    }

    return 0;
}
