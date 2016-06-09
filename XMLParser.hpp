#ifndef XMLParser_hpp_
#define XMLParser_hpp_

#include "fmt/format.h"
#include "pugixml.hpp"
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace zillow {
    using EdgeData = std::tuple<unsigned long, unsigned long, double>;

    enum ADDRESS {
        STREET = 0,
        ZIPCODE = 1,
        CITY = 2,
        STATE = 3,
        LATITUDE = 4,
        LONGITUDE = 5
    };
    using Address =
        std::tuple<std::string, int, std::string, std::string, double, double>;

    enum LINKS {
        HOMEDETAILS = 0,
        GRAPHSANDDATA = 1,
        MAPTHISHOME = 2,
        COMPARABLES = 3
    };
    using Links =
        std::tuple<std::string, std::string, std::string, std::string>;

    enum ZESTIMATE {
        CURRENCY = 0,
        AMOUNT = 1,
        LOW = 2,
        HIGH = 3,
        LASTUPDATED = 4
    };
    using Zestimate =
        std::tuple<std::string, double, double, double, std::string>;

    enum HOUSEGENERALINFO {
        USECODE = 0,
        YEARBUILT = 1,
        LOTSIZESQFT = 2,
        FINISHEDSQFT = 3,
        BATHROOMS = 4,
        BEDROOMS = 5,
        TOTALROOMS = 6
    };
    using HouseGeneralInfo =
        std::tuple<std::string, int, double, double, double, int, int>;

    enum TAXINFO { TAX_ASSESSMENT_YEAR = 0, TAX_ASSESSMENT = 1 };
    using TaxInfo = std::tuple<int, double>;

    enum HOUSESALERECORD {
        LAST_SOLD_DATE = 0,
        LAST_SOLD_PRICE = 1,
        LAST_SOLD_PRICE_CURRENCY = 2
    };
    using HouseSaleRecord = std::tuple<std::string, double, std::string>;

    enum QUERYREQUEST { TEXT = 0, CODE = 1 };
    using QueryRequest = std::tuple<std::string, std::string>;

    struct DeepSearchResults {
        unsigned long zpid;
        Links links;
        Address address;
        Zestimate zestimate;
        HouseGeneralInfo info;
        TaxInfo tax;
        HouseSaleRecord saleRecord;
    };

    using HashTable = std::unordered_map<std::string, std::string>;

    // Functions which can be used to display results.
    template <typename T> void print(const T &data);

    template <> void print<HashTable>(const HashTable &data) {
        std::map<std::string, std::string> sortedData(data.begin(), data.end());
        for (auto const &item : sortedData) {
            fmt::print("\"{0}\" - {1}\n", item.first, item.second);
        }
    }

    template <> void print<EdgeData>(const EdgeData &data) {
        fmt::print("{0} <--> {1} with score = {2}\n", std::get<0>(data),
                   std::get<1>(data), std::get<2>(data));
    }

    template <> void print<DeepSearchResults>(const DeepSearchResults &data) {
        fmt::MemoryWriter writer;
        // Display zpid
        writer << "zpid: \t" << data.zpid << "\n";

        // Display links
        writer << "links:\n"
               << "\thomedetails: " << std::get<HOMEDETAILS>(data.links) << "\n"
               << "\tgraphanddata: " << std::get<GRAPHSANDDATA>(data.links)
               << "\n"
               << "\tmapthishome: " << std::get<MAPTHISHOME>(data.links) << "\n"
               << "\tcomparables: " << std::get<COMPARABLES>(data.links)
               << "\n";

        // Display address
        writer << "address: \n"
               << "\tstreet: " << std::get<STREET>(data.address) << "\n"
               << "\tzipcode: " << std::get<ZIPCODE>(data.address) << "\n"
               << "\tcity: " << std::get<CITY>(data.address) << "\n"
               << "\tstate: " << std::get<STATE>(data.address) << "\n"
               << "\tlatitude: " << std::get<LATITUDE>(data.address) << "\n"
               << "\tlongitude: " << std::get<LONGITUDE>(data.address) << "\n";

        // Display other information
        writer << "useCode: " << std::get<USECODE>(data.info) << "\n";
        writer << "yearBuilt: " << std::get<YEARBUILT>(data.info) << "\n";
        writer << "lotSizeSqFt: " << std::get<LOTSIZESQFT>(data.info) << "\n";
        writer << "finishedSqFt: " << std::get<FINISHEDSQFT>(data.info) << "\n";
        writer << "bathrooms: " << std::get<BATHROOMS>(data.info) << "\n";
        writer << "bedrooms: " << std::get<BEDROOMS>(data.info) << "\n";
        writer << "totalRooms: " << std::get<TOTALROOMS>(data.info) << "\n";

        writer << "taxAssessmentYear: "
               << std::get<TAX_ASSESSMENT_YEAR>(data.tax) << "\n";
        writer << "taxAssessment: " << std::get<TAX_ASSESSMENT>(data.tax)
               << "\n";

        writer << "lastSoldDate: " << std::get<LAST_SOLD_DATE>(data.saleRecord)
               << "\n";
        writer << "lastSoldPrice: "
               << std::get<LAST_SOLD_PRICE>(data.saleRecord) << "\n";
        writer << "lastSoldPriceCurrency: "
               << std::get<LAST_SOLD_PRICE_CURRENCY>(data.saleRecord) << "\n";

        // Display zestimate
        writer << "zestimate: "
               << "\n\tcurrency: " << std::get<CURRENCY>(data.zestimate)
               << "\n\tamount: " << std::get<AMOUNT>(data.zestimate)
               << "\n\tlow: " << std::get<LOW>(data.zestimate)
               << "\n\thigh: " << std::get<HIGH>(data.zestimate)
               << "\n\tlast-updated: " << std::get<LASTUPDATED>(data.zestimate)
               << "\n";

        // Output to the stdout.
        fmt::print("{}", writer.str());
    }

    class NodeParser {
      public:
        NodeParser(pugi::xml_node root) { traverse(root, ""); }

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
        return std::make_tuple(data.find("/links/homedetails/")->second,
                               data.find("/links/graphsanddata/")->second,
                               data.find("/links/mapthishome/")->second,
                               data.find("/links/comparables/")->second);
    }

    /**
     * Parse address information.
     *
     * @param aNode
     *
     * @return
     */
    Address parseAddress(const HashTable &data) {
        return std::make_tuple(
            data.find("/address/street/")->second,
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
    Zestimate parseZestimate(const HashTable &data) {
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

        return std::make_tuple(currency, amount, low, high, lastUpdated);
    }

    HouseSaleRecord parseSaleRecord(const HashTable &data) {
        HashTable::const_iterator it;

        it = data.find("/lastSoldDate/");
        std::string lastSoldDate =
            (it != data.end()) ? it->second : "01/01/1900";

        it = data.find("/lastSoldPrice/");
        double lastSoldPrice = (it != data.end()) ? std::stod(it->second) : 0.0;

        it = data.find("/lastSoldPriceCurrency/");
        std::string lastSoldPriceCurrency =
            (it != data.end()) ? it->second : "USD";
        return std::make_tuple(lastSoldDate, lastSoldPrice,
                               lastSoldPriceCurrency);
    }

    TaxInfo parseTaxInfo(const HashTable &data) {
        HashTable::const_iterator it;
        it = data.find("/taxAssessmentYear/");
        int taxAssessmentYear =
            (it != data.end()) ? std::stoi(it->second) : 1900;

        it = data.find("/taxAssessment/");
        double taxAssessment = (it != data.end()) ? std::stod(it->second) : 0.0;

        return std::make_tuple(taxAssessmentYear, taxAssessment);
    }

    HouseGeneralInfo parseHouseGeneralInfo(const HashTable &data) {
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

        return std::make_tuple(useCode, yearBuilt, lotSizeSqFt, finishedSqFt,
                               bathrooms, bedrooms, totalRooms);
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

    bool validateData(const HashTable &data) {
        bool isValid =
            ((data.size() == 40) || (data.size() == 41) || (data.size() == 42));
        if (!isValid) {
            fmt::print("==============================\n");
            print(data);
        }
        return isValid;
    }

    DeepSearchResults extractData(const HashTable &data) {
        DeepSearchResults results;

        // Make sure that our code is in sync with the latest results.
        assert(validateData(data));

        // Get zpid
        HashTable::const_iterator it;
        it = data.find("/zpid/");
        assert(it != data.end()); // This field must exist
        results.zpid = std::stoul(it->second);

        results.links = parseLinks(data);
        results.address = parseAddress(data);
        results.zestimate = parseZestimate(data);
        results.info = parseHouseGeneralInfo(data);
        results.tax = parseTaxInfo(data);
        results.saleRecord = parseSaleRecord(data);

        return results;
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
        results.emplace_back(std::move(principal));

        // Get the comparison
        pugi::xml_node comps = rootNode.child("comparables");
        for (pugi::xml_node aChild = comps.first_child(); aChild;
             aChild = aChild.next_sibling()) {
            NodeParser parser(aChild);
            auto const &data = parser.getData();
            auto info = parseDeepSearchResultsResponse(aChild);
            HashTable::const_iterator it = data.find("/score");
            assert(it != data.end()); // score property must exist.
            edges.emplace_back(std::make_tuple(principal_zpid, info.zpid,
                                               std::stod(it->second)));
            results.emplace_back(std::move(info));
        }

        // Return
        return std::make_tuple(results, edges);
    }
}

#endif
