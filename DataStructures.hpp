#ifndef DataStructures_hpp_
#define DataStructures_hpp_

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "fmt/format.h"

// Cereal
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/bitset.hpp"
#include "cereal/types/chrono.hpp"
#include "cereal/types/complex.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/forward_list.hpp"
#include "cereal/types/queue.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/utility.hpp"
#include "cereal/types/valarray.hpp"
#include "cereal/types/vector.hpp"

namespace zillow {
    using IDType = unsigned int;
    using Real = double;
    using HashTable = std::unordered_map<std::string, std::string>;

    struct EdgeData {
        explicit EdgeData(const IDType srcId, const IDType dstId, Real score)
            : SrcID(srcId), DstID(dstId), Score(score) {}

        IDType SrcID;
        IDType DstID;
        Real Score;

        EdgeData(const EdgeData &data)
            : SrcID(data.SrcID), DstID(data.DstID), Score(data.Score){};

        EdgeData(EdgeData &&data)
            : SrcID(data.SrcID), DstID(data.DstID), Score(data.Score){};

        EdgeData & operator=(EdgeData rhs) {
            std::swap(SrcID, rhs.SrcID);
            std::swap(DstID, rhs.DstID);
            std::swap(Score, rhs.Score);
            return *this;
        }

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("srcid", SrcID), cereal::make_nvp("dstid", DstID),
               cereal::make_nvp("score", Score));
        }
    };

    struct Address {
        explicit Address(const std::string &street, const int zipcode, const std::string &city,
                         const std::string &state, const Real latitude, const Real longitude)
            : Street(street), ZipCode(zipcode), City(city), State(state), Latitude(latitude),
              Longitude(longitude) {}

        std::string Street;
        size_t ZipCode;
        std::string City;
        std::string State;
        Real Latitude;
        Real Longitude;

        Address(const Address &data)
            : Street(data.Street), ZipCode(data.ZipCode), City(data.City), State(data.State),
              Latitude(data.Latitude), Longitude(data.Longitude) {}

        Address(Address &&data) noexcept : Street(std::move(data.Street)),
                                           ZipCode(data.ZipCode),
                                           City(std::move(data.City)),
                                           State(std::move(data.State)),
                                           Latitude(data.Latitude),
                                           Longitude(data.Longitude) {}

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("street", Street), cereal::make_nvp("zipcode", ZipCode),
               cereal::make_nvp("city", City), cereal::make_nvp("state", State),
               cereal::make_nvp("latitude", Latitude),
               cereal::make_nvp("longitude", Longitude));
        }
    };

    struct Links {
        explicit Links(const std::string &homedetails, const std::string &graphanddata,
                       const std::string &mapthishome, const std::string &comparables)
            : HomeDetails(homedetails), GraphAndData(graphanddata), MapThisHome(mapthishome),
              Comparables(comparables) {}

        Links(const Links &data)
            : HomeDetails(data.HomeDetails), GraphAndData(data.GraphAndData),
              MapThisHome(data.MapThisHome), Comparables(data.Comparables) {}

        Links(Links &&data) noexcept : HomeDetails(std::move(data.HomeDetails)),
                                       GraphAndData(std::move(data.GraphAndData)),
                                       MapThisHome(std::move(data.MapThisHome)),
                                       Comparables(std::move(data.Comparables)) {}

        std::string HomeDetails;
        std::string GraphAndData;
        std::string MapThisHome;
        std::string Comparables;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("homedetails", HomeDetails),
               cereal::make_nvp("graphsanddata", GraphAndData),
               cereal::make_nvp("mapthishome", MapThisHome),
               cereal::make_nvp("comparables", Comparables));
        }
    };

    struct ZEstimate {
        explicit ZEstimate()
            : Amount(), Low(), High(), Currency("USB"), LastUpdated("01/01/1990"){};

        explicit ZEstimate(Real amount, Real low, Real high, const std::string &currency,
                           const std::string &last_updated)
            : Amount(amount), Low(low), High(high), Currency(currency),
              LastUpdated(last_updated) {}
        Real Amount;
        Real Low;
        Real High;
        std::string Currency;
        std::string LastUpdated;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("currency", Currency), cereal::make_nvp("amount", Amount),
               cereal::make_nvp("low", Low), cereal::make_nvp("high", High),
               cereal::make_nvp("last_updated", LastUpdated));
        }
    };

    struct HouseInfo {
        explicit HouseInfo(const Address &address, const std::string &useCode,
                           const int yearBuilt, Real lotSizeSqFt, Real finishedSqFt,
                           Real bathrooms, int bedrooms, int totalRooms)
            : HouseAddress(address), UseCode(useCode), YearBuilt(yearBuilt),
              LotSizeSqFt(lotSizeSqFt), FinishedSqFt(finishedSqFt), Bathrooms(bathrooms),
              Bedrooms(bedrooms), TotalRooms(totalRooms) {}
        Address HouseAddress;
        std::string UseCode;
        int YearBuilt;
        Real LotSizeSqFt;
        Real FinishedSqFt;
        Real Bathrooms;
        int Bedrooms;
        int TotalRooms;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("address", HouseAddress), cereal::make_nvp("useCode", UseCode),
               cereal::make_nvp("yearBuilt", YearBuilt),
               cereal::make_nvp("lotSizeSqFt", LotSizeSqFt),
               cereal::make_nvp("finishedSqFt", FinishedSqFt),
               cereal::make_nvp("bathrooms", Bathrooms), cereal::make_nvp("bedrooms", Bedrooms),
               cereal::make_nvp("totalRooms", TotalRooms));
        }
    };

    struct TaxInfo {
        explicit TaxInfo() : TaxAssessmentYear(1990), TaxAssessment(0.0) {}
        explicit TaxInfo(const int year, const Real assessment)
            : TaxAssessmentYear(year), TaxAssessment(assessment) {}
        int TaxAssessmentYear;
        Real TaxAssessment;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("taxAssessmentYear", TaxAssessmentYear),
               cereal::make_nvp("taxAssessment", TaxAssessment));
        }
    };

    struct SaleRecord {
        explicit SaleRecord()
            : LastSoldDate("01/01/1990"), LastSoldPrice(0.0), Currency("USD") {}
        explicit SaleRecord(const std::string &soldDate, const Real soldPrice,
                            const std::string &currency)
            : LastSoldDate(soldDate), LastSoldPrice(soldPrice), Currency(currency) {}

        std::string LastSoldDate;
        Real LastSoldPrice;
        std::string Currency;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("lastSoldDate", LastSoldDate),
               cereal::make_nvp("lastSoldPrice", LastSoldPrice),
               cereal::make_nvp("Currency", Currency));
        }
    };

    struct QueryRequest {
        explicit QueryRequest(const std::string &street, const std::string &citystatezip)
            : Street(street), CityStateZip(citystatezip) {}
        std::string Street;
        std::string CityStateZip;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("street", Street),
               cereal::make_nvp("citystatezip", CityStateZip));
        }
    };

    struct Message {
        explicit Message(const std::string &text, const int code) : Text(text), Code(code) {}
        std::string Text;
        int Code;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("Text", Text), cereal::make_nvp("Code", Code));
        }
    };

    struct DeepSearchResults {
        explicit DeepSearchResults(unsigned long zid, const HouseInfo &hinfo,
                                   const Links &zlinks, const TaxInfo &tinfo,
                                   const SaleRecord &srecord, const ZEstimate &zes)
            : zpid(zid), info(hinfo), links(zlinks), tax(tinfo), saleRecord(srecord),
              zestimate(zes) {}
        unsigned long zpid;
        HouseInfo info;
        Links links;
        TaxInfo tax;
        SaleRecord saleRecord;
        ZEstimate zestimate;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("zpid", zpid), cereal::make_nvp("links", links),
               cereal::make_nvp("HouseInfo", info), cereal::make_nvp("Tax", tax),
               cereal::make_nvp("SaleRecord", saleRecord),
               cereal::make_nvp("zestimate", zestimate));
        }
    };

    struct ImageLinks {
        std::string PhotoGalery;
        std::vector<std::string> Images;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("PhotoGalery", PhotoGalery),
               cereal::make_nvp("Images", Images));
        }
    };

    using PageViewCount = std::array<int, 2>;

    struct EditedFacts {
        std::string UseCode;
        Real Bathrooms;
        int Bedrooms;
        int TotalRooms;
        Real LotSizeSqFt;
        Real FinishedSqFt;
        Real YearBuilt;
        std::string Appliances;
        std::string HomeDescriptions;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("UseCode", UseCode), cereal::make_nvp("Bathrooms", Bathrooms),
               cereal::make_nvp("Bedrooms", Bedrooms),
               cereal::make_nvp("TotalRooms", TotalRooms),
               cereal::make_nvp("LotSizeSqFt", LotSizeSqFt),
               cereal::make_nvp("FinishedSqFt", FinishedSqFt),
               cereal::make_nvp("YearBuilt", YearBuilt),
               cereal::make_nvp("Appliances", Appliances),
               cereal::make_nvp("HomeDescriptions", HomeDescriptions));
        }
    };

    struct UpdatedPropertyDetails {
        IDType zpid;
        PageViewCount pageViewCount;
        Address address;
        EditedFacts editedFacts;
    };
}

#endif
