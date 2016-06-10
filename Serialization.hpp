#ifndef Serialization_hpp_
#define Serialization_hpp_
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

#include "XMLParser.hpp"

namespace zillow {
    template <typename OArchive, typename OutputStream>
    void print(OutputStream &output, const DeepSearchResults &data) {
        OArchive oar(output);
        oar(cereal::make_nvp("zpid", data.zpid),
            cereal::make_nvp("street", std::get<STREET>(data.address)),
            cereal::make_nvp("city", std::get<CITY>(data.address)),
            cereal::make_nvp("state", std::get<STATE>(data.address)),
            cereal::make_nvp("zipcode", std::get<ZIPCODE>(data.address)),
            cereal::make_nvp("latitude", std::get<LATITUDE>(data.address)),
            cereal::make_nvp("longitude", std::get<LONGITUDE>(data.address)),
            cereal::make_nvp("homedetails", std::get<HOMEDETAILS>(data.links)),
            cereal::make_nvp("graphanddata", std::get<GRAPHSANDDATA>(data.links)),
            cereal::make_nvp("mapthishome", std::get<MAPTHISHOME>(data.links)),
            cereal::make_nvp("comparables", std::get<COMPARABLES>(data.links)),
            cereal::make_nvp("useCode", std::get<USECODE>(data.info)),
            cereal::make_nvp("yearBuilt", std::get<YEARBUILT>(data.info)),
            cereal::make_nvp("lostSizeSqFt", std::get<LOTSIZESQFT>(data.info)),
            cereal::make_nvp("finishedSqFt", std::get<FINISHEDSQFT>(data.info)),
            cereal::make_nvp("bathrooms", std::get<BATHROOMS>(data.info)),
            cereal::make_nvp("bedrooms", std::get<BEDROOMS>(data.info)),
            cereal::make_nvp("totalRooms", std::get<TOTALROOMS>(data.info)),           
            cereal::make_nvp("taxAssessmentYear", std::get<TAX_ASSESSMENT_YEAR>(data.tax)), 
            cereal::make_nvp("taxAssessment", std::get<TAX_ASSESSMENT>(data.tax)), 
            cereal::make_nvp("lastSoldDate", std::get<LAST_SOLD_DATE>(data.saleRecord)), 
            cereal::make_nvp("lastSoldPrice", std::get<LAST_SOLD_PRICE>(data.saleRecord)), 
            cereal::make_nvp("lastSoldPriceCurrency", std::get<LAST_SOLD_PRICE_CURRENCY>(data.saleRecord)), 
            cereal::make_nvp("estimate-currency", std::get<ZESTIMATE_CURRENCY>(data.zestimate)),
            cereal::make_nvp("estimate-amount", std::get<ZESTIMATE_AMOUNT>(data.zestimate)),
            cereal::make_nvp("estimate-low", std::get<ZESTIMATE_LOW>(data.zestimate)),
            cereal::make_nvp("estimate-high", std::get<ZESTIMATE_HIGH>(data.zestimate)),
            cereal::make_nvp("last-updated", std::get<ZESTIMATE_LAST_UPDATED>(data.zestimate)));
    }    

    template <typename OArchive, typename OutputStream>
    void print(OutputStream &output, const EdgeData &data) {
        OArchive oar(output);
        oar(cereal::make_nvp("src_id", std::get<SRC_ID>(data)),
            cereal::make_nvp("dst_id", std::get<DST_ID>(data)),
            cereal::make_nvp("score", std::get<SCORE>(data)));
    }    

    // TODO: Use cereal to output in JSON format.
    template <typename T> void print(const T &data);

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
               << "\n\tcurrency: " << std::get<ZESTIMATE_CURRENCY>(data.zestimate)
               << "\n\tamount: " << std::get<ZESTIMATE_AMOUNT>(data.zestimate)
               << "\n\tlow: " << std::get<ZESTIMATE_LOW>(data.zestimate)
               << "\n\thigh: " << std::get<ZESTIMATE_HIGH>(data.zestimate)
               << "\n\tlast-updated: " << std::get<ZESTIMATE_LAST_UPDATED>(data.zestimate)
               << "\n";

        // Output to the stdout.
        fmt::print("{}", writer.str());
    }
}
#endif
