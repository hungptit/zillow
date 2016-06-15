#ifndef Zillow_hpp_
#define Zillow_hpp_

#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

namespace zillow {
    enum EDGEDATA { SRC_ID, DST_ID, SCORE, EDGEDATA_LEN };
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
        ZESTIMATE_CURRENCY = 0,
        ZESTIMATE_AMOUNT = 1,
        ZESTIMATE_LOW = 2,
        ZESTIMATE_HIGH = 3,
        ZESTIMATE_LAST_UPDATED = 4
    };
    using Zestimate =
        std::tuple<std::string, double, double, double, std::string>;

    enum HOUSE_GENERAL_INFO {
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

    enum TAXINFO { TAX_ASSESSMENT_YEAR, TAX_ASSESSMENT };
    using TaxInfo = std::tuple<int, double>;

    enum HOUSE_SALE_RECORD {
        LAST_SOLD_DATE = 0,
        LAST_SOLD_PRICE = 1,
        LAST_SOLD_PRICE_CURRENCY = 2
    };
    using HouseSaleRecord = std::tuple<std::string, double, std::string>;

    enum QUERY_REQUEST { QUERY_STREET = 0, QUERY_CITYSTATEZIP = 1 };
    using QueryRequest = std::tuple<std::string, std::string>;

    enum MESSAGE { TEXT = 0, CODE = 1 };
    using Message = std::tuple<std::string, std::string>;

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
}
#endif
