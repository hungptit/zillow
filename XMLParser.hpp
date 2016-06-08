#ifndef XMLParser_hpp_
#define XMLParser_hpp_

#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace zillow {
    enum ADDRESS {
        STREET = 0,
        CITY = 1,
        STATE = 2,
        LATITUDE = 3,
        LONGITUDE = 4
    };
    using Address =
        std::tuple<std::string, std::string, std::string, double, double>;

    enum LINKS {
        HOMEDETAILS = 0,
        GRAPHSANDDATA = 1,
        MAPTHISHOME = 2,
        COMPARABLES = 3
    };
    using Links =
        std::tuple<std::string, std::string, std::string, std::string>;

    using Zestimate = std::tuple<std::string, double, double, double>;

    enum QUERYREQUEST { TEXT = 0, CODE = 1 };
    using QueryRequest = std::tuple<std::string, std::string>;
    struct DeepSearchResults {
        size_t zpid;
        Links links;
        Address address;
        int FIPScounty;
        std::string useCode;
        int taxAssessmentYear;
        double taxAssessment;
        int yearBuilt;
        double lotSizeSqFt;
        double finishedSqFt;
        double bathrooms;
        int bedrooms;
        int totalRooms;
        std::string lastSoldDate;
        double lastSoldPrice;
        Zestimate zestimate;
    };

    class NodeParser {
      public:
        NodeParser(pugi::xml_node root) { traverse(root, ""); }

        const std::unordered_map<std::string, std::string> getData() const {
            return Data;
        }

      private:
        void traverse(pugi::xml_node root, const std::string &prefix) {
            for (pugi::xml_node aChild = root.first_child(); aChild;
                 aChild = aChild.next_sibling()) {
                std::string aKey = prefix + "/" + aChild.name();
                if (aChild.type() == pugi::node_pcdata) {
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

        std::unordered_map<std::string, std::string> Data;
    };

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
    Links parseLinks(pugi::xml_node aNode) {}

    /**
     * Parse address information.
     *
     * @param aNode
     *
     * @return
     */
    Address parseAddress(pugi::xml_node aNode) {}

    /**
     * Get zestimate information for a given zestimate node.
     *
     * @param aNode
     *
     * @return
     */
    Zestimate parseZestimate(pugi::xml_node aNode) {}

    /**
     * Parse deep search results.
     *
     * @param aNode
     *
     * @return
     */
    DeepSearchResults parseDeepSearchResults(pugi::xml_node aNode) {}

    /**
     * Parse the deep search results which has a list of deep search results.
     *
     * @param aNode
     *
     * @return
     */
    std::vector<DeepSearchResults> parseCompsResults(pugi::xml_node aNode) {}

    auto parseRequest(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        auto const &data = parser.getData();
        assert(data.size() == 2);
        auto const addressIter = data.find("/address/");
        auto const citystatezipIter = data.find("/citystatezip/");
        return std::make_tuple(addressIter->second, citystatezipIter->second);
    }

    auto parseMessage(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        auto const &data = parser.getData();
        assert(data.size() == 2);
        auto const textIter = data.find("/text/");
        auto const codeIter = data.find("/code/");
        assert(textIter != data.end());
        assert(codeIter != data.end());
        return std::make_tuple(textIter->second, std::stoi(codeIter->second));
    }

    auto parseResponse(pugi::xml_node rootNode) {
        NodeParser parser(rootNode);
        auto const &data = parser.getData();
        for (auto const &item : data) {
            fmt::print("{0} : {1}\n", item.first, item.second);
        }
        DeepSearchResults results;
        using Containter = decltype(data);

        // Update results
        {
            auto const it = data.find("/results/result/zpid/");
            assert(it != data.end());
            results.zpid = std::stoul(it->second);
        }

        // Update
        {
            auto const it = data.find("/results/result/useCode/");
            assert(it != data.end());
            results.useCode = it->second;
        }

        {
            auto const it = data.find("/results/result/taxAssessmentYear/");
            if (it != data.end()) {
                results.taxAssessmentYear = std::stoi(it->second);
            } else {
                results.taxAssessmentYear = 0;
            }
        }

        {
            auto const it = data.find("/results/result/taxAssessment/");
            if (it != data.end()) {
                results.taxAssessment = std::stod(it->second);
            } else {
                results.taxAssessment = 0.0;
            }
        }

        return results;
    }

    template <typename T> void print(const T &data);

    template <> void print<DeepSearchResults>(const DeepSearchResults &data) {
        fmt::MemoryWriter writer;
        // Display zpid
        writer << "zpid: \t" << data.zpid << "\n";

        // Display links
        writer << "links:\n";
        writer << "homedetail: \n"
               << "\thomedetails: " << std::get<HOMEDETAILS>(data.links) << "\n"
               << "\tgraphanddata: " << std::get<GRAPHSANDDATA>(data.links)
               << "\n"
               << "\tmapthishome: " << std::get<MAPTHISHOME>(data.links) << "\n"
               << "\tcomparables: " << std::get<COMPARABLES>(data.links)
               << "\n";
        // Display address
        writer << "address: \n"
               << "\tstreet: " << std::get<STREET>(data.address) << "\n"
               << "\tcity: " << std::get<CITY>(data.address) << "\n"
               << "\tstate: " << std::get<STATE>(data.address) << "\n"
               << "\tlatitude: " << std::get<LATITUDE>(data.address) << "\n"
               << "\tlongitude: " << std::get<LONGITUDE>(data.address) << "\n";

        // Display other information
        writer << "address: " << data.useCode << "\n";
        writer << "taxAssessmentYear: " << data.taxAssessmentYear << "\n";
        writer << "taxAssessment: " << data.taxAssessment << "\n";
        writer << ": " << data.yearBuilt << "\n";
        writer << ": " << data.lotSizeSqFt << "\n";
        writer << ": " << data.finishedSqFt << "\n";
        writer << ": " << data.bathrooms << "\n";
        writer << ": " << data.bedrooms << "\n";
        writer << ": " << data.totalRooms << "\n";
        writer << ": " << data.lastSoldDate << "\n";
        writer << ": " << data.lastSoldPrice << "\n";
        writer << "zestimate: \t"
               << "\n";
        writer << "localRealEstate: \t"
               << "\n";
        fmt::print("{}", writer.str());
    }
}

#endif
