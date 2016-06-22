#ifndef XMLParser_hpp_
#define XMLParser_hpp_

#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Zillow.hpp"

namespace zillow {
    class NodeParser {
      public:
        explicit NodeParser(pugi::xml_node root) { traverse(root, ""); }

        const HashTable getData() const { return Data; }

      private:
        void traverse(pugi::xml_node root, const std::string &prefix) {
            for (pugi::xml_node aChild = root.first_child(); aChild;
                 aChild = aChild.next_sibling()) {
                std::string aKey = prefix + "/" + aChild.name();
                if (aChild.type() == pugi::node_pcdata) {
                    assert(Data.find(aKey) == Data.end()); // This function
                                                           // cannot handle
                                                           // duplicated key.
                    Data[aKey] = aChild.value();
                } else {
                    traverse(aChild, aKey);
                }
            }

            // Get attributes for this node.
            for (pugi::xml_attribute aChild = root.first_attribute(); aChild;
                 aChild = aChild.next_attribute()) {
                std::string aKey = prefix + "/" + aChild.name();
                Data[aKey] = aChild.value();
            }
        }

        HashTable Data;
    };

    /**
     * This API will be used for debugging purpose.
     *
     * @param aNode
     * @param prefix
     */
    void dfs(pugi::xml_node aNode, const std::string &prefix) {
        for (pugi::xml_node aChild = aNode.first_child(); aChild;
             aChild = aChild.next_sibling()) {
            const std::string nodeName = aChild.name();
            if (aChild.type() == pugi::node_pcdata) {
                std::cout << "Type = " << aChild.type() << " "
                          << prefix + "/ --> " + nodeName << ":"
                          << aChild.value() << "\n";
            } else {
                std::cout << "Type = " << aChild.type() << " "
                          << prefix + "/ --> " + nodeName << ":"
                          << aChild.value() << "\n";
            }

            for (pugi::xml_attribute anAttribute = aNode.first_attribute();
                 anAttribute; anAttribute = anAttribute.next_attribute()) {
                std::cout << "Attribute -> " << anAttribute.name() << ":"
                          << anAttribute.value() << "\n";
            }
            dfs(aChild, prefix + "/" + nodeName);
        }
    }

    /**
     * Parse links information.
     *
     * @param aNode
     *
     * @return
     */
    Links parseLinks(const HashTable &data) {
        HashTable::const_iterator it;

        it = data.find("/links/homedetails/");
        std::string homedetails = (it != data.end()) ? it->second : "";

        it = data.find("/links/graphsanddata/");
        std::string graphsanddata = (it != data.end()) ? it->second : "";

        it = data.find("/links/mapthishome/");
        std::string mapthishome = (it != data.end()) ? it->second : "";

        it = data.find("/links/comparables/");
        std::string comparables = (it != data.end()) ? it->second : "";

        return Links(homedetails, graphsanddata, mapthishome, comparables);
    }

    /**
     * Parse address information.
     *
     * @param aNode
     *
     * @return
     */
    Address parseAddress(const HashTable &data) {
        return Address(data.find("/address/street/")->second,
                       std::stoi(data.find("/address/zipcode/")->second),
                       data.find("/address/city/")->second,
                       data.find("/address/state/")->second,
                       std::stod(data.find("/address/latitude/")->second),
                       std::stod(data.find("/address/longitude/")->second));
    }

    /**
     * Get zestimate information for a given zestimate node.
     *
     * @param aNode
     *
     * @return
     */
    ZEstimate parseZestimate(const HashTable &data) {
        HashTable::const_iterator it;

        it = data.find("/zestimate/amount/");
        double amount = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/zestimate/valuationRange/high/");
        double high = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/zestimate/valuationRange/low/");
        double low = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/zestimate/last-updated/");
        std::string lastUpdated = (it != data.end()) ? it->second : "";

        it = data.find("/zestimate/amount/currency");
        std::string currency = (it != data.end()) ? it->second : "USD";

        return ZEstimate(amount, low, high, currency, lastUpdated);
    }

    SaleRecord parseSaleRecord(const HashTable &data) {
        HashTable::const_iterator it;

        it = data.find("/lastSoldDate/");
        std::string lastSoldDate =
            (it != data.end()) ? it->second : "01/01/1990";

        it = data.find("/lastSoldPrice/");
        double lastSoldPrice = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/lastSoldPriceCurrency/");
        std::string lastSoldPriceCurrency =
            (it != data.end()) ? it->second : "USD";
        return SaleRecord(lastSoldDate, lastSoldPrice, lastSoldPriceCurrency);
    }

    TaxInfo parseTaxInfo(const HashTable &data) {
        HashTable::const_iterator it;
        it = data.find("/taxAssessmentYear/");
        int taxAssessmentYear =
            (it != data.end()) ? std::stoi(it->second) : 1990;

        it = data.find("/taxAssessment/");
        double taxAssessment = (it != data.end()) ? std::stod(it->second) : 0.0;

        return TaxInfo(taxAssessmentYear, taxAssessment);
    }

    HouseInfo parseHouseGeneralInfo(const HashTable &data) {
        HashTable::const_iterator it;

        it = data.find("/useCode/");
        std::string useCode = (it != data.end()) ? it->second : "";

        it = data.find("/yearBuilt/");
        int yearBuilt = (it != data.end()) ? std::stoi(it->second) : 1900;

        it = data.find("/lotSizeSqFt/");
        double lotSizeSqFt = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/finishedSqFt/");
        double finishedSqFt = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/bathrooms/");
        double bathrooms = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/bedrooms/");
        int bedrooms = (it != data.end()) ? std::stod(it->second) : 0;

        it = data.find("/totalRooms/");
        int totalRooms = (it != data.end()) ? std::stod(it->second) : 0;

        return HouseInfo(parseAddress(data), useCode, yearBuilt, lotSizeSqFt,
                         finishedSqFt, bathrooms, bedrooms, totalRooms);
    }

    auto parseDeepSearchResultsRequest(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        auto const &data = parser.getData();
        assert(data.size() == 2);
        auto const addressIter = data.find("/address/");
        auto const citystatezipIter = data.find("/citystatezip/");
        return std::make_tuple(addressIter->second, citystatezipIter->second);
    }

    auto parseDeepCompsRequest(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        auto const &data = parser.getData();
        assert(data.size() == 2);
        return std::make_tuple(std::stoul(data.find("/zpid/")->second),
                               std::stoi(data.find("/count/")->second));
    }

    Message parseMessage(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        auto const &data = parser.getData();
        assert(data.size() == 2);
        auto const textIter = data.find("/text/");
        auto const codeIter = data.find("/code/");
        assert(textIter != data.end());
        assert(codeIter != data.end());
        return Message(textIter->second, std::stoi(codeIter->second));
    }

    bool validateData(const HashTable &data) {
        bool isValid = ((data.size() > 30) && (data.size() < 44));
        if (!isValid) {
            fmt::print("==============================\n");
            fmt::print("Number of elements: {}\n", data.size());
            std::map<std::string, std::string> sortedData(data.begin(),
                                                          data.end());
            for (auto const &item : sortedData) {
                fmt::print("\"{0}\" - {1}\n", item.first, item.second);
            }
        } else {
            // fmt::print("==============================\n");
            // fmt::print("Number of elements: {}\n", data.size());
            // std::map<std::string, std::string> sortedData(data.begin(),
            // data.end());
            // for (auto const &item : sortedData) {
            //     fmt::print("\"{0}\" - {1}\n", item.first, item.second);
            // }
        }
        return isValid;
    }

    DeepSearchResults extractData(const HashTable &data) {
        // Make sure that our code is in sync with the latest results.
        assert(validateData(data));

        // Get zpid
        HashTable::const_iterator it;
        it = data.find("/zpid/");
        assert(it != data.end()); // This field must exist
        auto zpid = std::stoul(it->second);

        return DeepSearchResults(zpid, parseHouseGeneralInfo(data),
                                 parseLinks(data), parseTaxInfo(data),
                                 parseSaleRecord(data), parseZestimate(data));
    }

    DeepSearchResults parseDeepSearchResultsResponse(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        return extractData(parser.getData());
    }

    std::tuple<std::vector<DeepSearchResults>, std::vector<EdgeData>>
    parseDeepCompsResponse(pugi::xml_node rootNode) {
        std::vector<DeepSearchResults> results;
        std::vector<EdgeData> edges;

        // Get the principal
        auto principal =
            parseDeepSearchResultsResponse(rootNode.child("principal"));
        auto principal_zpid = principal.zpid;

        // Get the comparison
        pugi::xml_node comps = rootNode.child("comparables");
        for (pugi::xml_node aChild = comps.first_child(); aChild;
             aChild = aChild.next_sibling()) {
            NodeParser parser(aChild);
            auto const &data = parser.getData();
            auto info = parseDeepSearchResultsResponse(aChild);
            HashTable::const_iterator it = data.find("/score");
            assert(it != data.end()); // score property must exist.
            edges.emplace_back(
                EdgeData(principal_zpid, info.zpid, std::stod(it->second)));
            results.emplace_back(std::move(info));
        }

        // Return
        return std::make_tuple(results, edges);
    }


  auto parseUpdatedPropertyDetails(pugi::xml_node rootNode) {
  }
}

#endif
