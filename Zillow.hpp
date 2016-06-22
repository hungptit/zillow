#ifndef Zillow_hpp_
#define Zillow_hpp_

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
    struct ZWSID {
        static const std::string ID1;
        static const std::string ID2;
    };

    const std::string ZWSID::ID1 = "X1-ZWz1fbpbq5pogb_3wguv";
    const std::string ZWSID::ID2 = "X1-ZWz1f8wdb88lxn_1y8f9";

    using IDType = unsigned int;
    using HashTable = std::unordered_map<std::string, std::string>;
    using Real = double;

    struct EdgeData {
        explicit EdgeData(const IDType srcId, const IDType dstId, Real score)
            : SrcID(srcId), DstID(dstId), Score(score) {}

        IDType SrcID;
        IDType DstID;
        Real Score;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("src_id", SrcID), cereal::make_nvp("dst_id", DstID),
               cereal::make_nvp("score", Score));
        }
    };

    struct Address {
        explicit Address(const std::string &street, const int zipcode, const std::string &city,
                         const std::string &state, const Real latitude, const Real longitude)
            : Street(street), ZipCode(zipcode), City(city), State(state), Latitude(latitude),
              Longitude(longitude) {}
        std::string Street;
        int ZipCode;
        std::string City;
        std::string State;
        Real Latitude;
        Real Longitude;

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
               cereal::make_nvp("house-info", info), cereal::make_nvp("tax-info", tax),
               cereal::make_nvp("sale-record", saleRecord),
               cereal::make_nvp("zestimate", zestimate));
        }
    };

    struct ImageLinks {
        std::string PhotoGalery;
        std::vector<std::string> Images;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("photo-galery", PhotoGalery),
               cereal::make_nvp("images", Images));
        }
    };

    using PageViewCount = std::array<int, 2>;

    struct EdditedFacts {
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
        ar(cereal::make_nvp("useCode", zpid),
           cereal::make_nvp("bathrooms", Bathrooms),
           cereal::make_nvp("bedrooms", Bedrooms),
           cereal::make_nvp("totalRooms", TotalRooms),
           cereal::make_nvp("house-info", info), cereal::make_nvp("tax-info", tax),
           cereal::make_nvp("sale-record", saleRecord),
           cereal::make_nvp("zestimate", zestimate));
      }
    };

    struct UpdatedPropertyDetails {
        IDType zpid;
        PageViewCount pageViewCount;
        Address address;
    };

    void strrep(std::string &s, char oldChar, char newChar) {
        const size_t N = s.size();
        for (size_t idx = 0; idx < N; ++idx) {
            if (s[idx] == oldChar) {
                s[idx] = newChar;
            }
        }
    }

    std::string generateDeepSearchQuery(const std::string &zwpid, const Address &info) {
        std::string aStreet(info.Street);
        strrep(aStreet, ' ', '+');
        std::string aCity(info.City);
        strrep(aCity, ' ', '+');

        return "http://www.zillow.com/webservice/"
               "GetDeepSearchResults.htm?zws-id=" +
               zwpid + "&address=" + aStreet + "&citystatezip=" + aCity + "%2C+" + info.State;
    }

    std::string generateDeepCompsQuery(const std::string &zwpid, const long zpid,
                                       const int count) {
        return "http://www.zillow.com/webservice/GetDeepComps.htm?zws-id=" + zwpid + "&zpid=" +
               std::to_string(zpid) + "&count=" + std::to_string(count);
    }

    std::string generateUpdatedPropertyDetailsQuery(const std::string &zwpid, const long zpid) {
        return "http://www.zillow.com/webservice/"
               "GetUpdatedPropertyDetails.htm?zws-id=" +
               zwpid + "&zpid=" + std::to_string(zpid);
    }

    class BasicConstraints {
      public:
        explicit BasicConstraints(const std::vector<std::string> &cities,
                                  std::vector<std::string> &states)
            : DesiredCities(cities.begin(), cities.end()),
              DesiredStates(states.begin(), states.end()) {}

        bool isValid(const Address &address) {
            fmt::print("Cities: ");
            for (const std::string &item : DesiredCities) {
                fmt::print("{} ", item);
            }
            fmt::print("\n");
            fmt::print("States: ");
            for (const std::string &item : DesiredStates) {
                fmt::print("{} ", item);
            }
            fmt::print("\n");

            return (DesiredStates.find(address.State) != DesiredStates.end()) &&
                   (DesiredCities.find(address.City) != DesiredCities.end());
        }

      private:
        std::unordered_set<std::string> DesiredCities;
        std::unordered_set<std::string> DesiredStates;
    };
}
#endif
