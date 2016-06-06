#include <string>
#include <vector>
#include <iostream>
#include "pugixml.hpp"
#include "fmt/format.h"

void dfs(pugi::xml_node aNode, const std::string prefix) {
  fmt::print("Node: {}\n", aNode.name());
  for (pugi::xml_node aChild = aNode.first_child(); aChild; aChild = aChild.next_sibling())
    {
      std::string nodeName = aChild.name();
      std::cout << " " <<  prefix + "/" + nodeName << "\n";
      dfs(aChild, prefix + "/" + nodeName);
    }
}

int main() {
  pugi::xml_document doc;
  pugi::xml_parse_result results = doc.load_file("deepSearchResults.xml");
  pugi::xml_node info = doc.child("SearchResults:searchresults");

  {
    for (pugi::xml_node item = info.first_child(); item; item = item.next_sibling())
      {
        std::cout << item.name() << std::endl;
        for (pugi::xml_node attr = item.first_child(); attr; attr = attr.next_sibling())
          {
            std::cout << " " << attr.name() << "=" << attr.text() << std::endl;
          }
        std::cout << std::endl;
      }
  }

   pugi::xpath_query query_name("/SearchResults:searchresults");
   pugi::xpath_query query_address("/SearchResults:searchresults/request/address");
   pugi::xpath_query query_zipcode("/SearchResults:searchresults/response/results/result/zpid");
   std::cout << "Query address: " << query_address.evaluate_string(doc) << "\n";
   std::cout << "Query zipcode: " << query_zipcode.evaluate_string(doc) << "\n";

   dfs(info, "/" + info.name());

   // TODO: DFS to find all data fields
   // Import 
  
  return 0;
}
