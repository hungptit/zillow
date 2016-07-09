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

// zillow
#include "XMLParser.hpp"

namespace zillow {
    template <typename OArchive> void print(const DeepSearchResults &data) {
        std::ostringstream output;
        {
            OArchive oar(output);
            const std::string aKey = data.info.HouseAddress.Street + ", " +
                                     data.info.HouseAddress.City + " " +
                                     data.info.HouseAddress.State + " " +
                                     data.info.HouseAddress.ZipCode;
            oar(cereal::make_nvp(aKey, data));
        }
        fmt::print("{}\n", output.str());
    }

    template <typename OArchive> void print(const std::vector<DeepSearchResults> &data) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("DeepComps results", data));
        }
        fmt::print("{}\n", output.str());
    }

    template <typename OArchive> void print(const UpdatedPropertyDetails &info) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Updated property details", info));
        }
        fmt::print("{}\n", output.str());
    }

    template <typename OArchive> void print(const std::vector<EdgeData> &data) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Edges", data));
        }
        fmt::print("{}\n", output.str());
    }

    template <typename OArchive> void print(const Message &data) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Message", data));
        }
        fmt::print("{}\n", output.str());
    }

    template <typename OArchive> void print(const Address &info) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Address", info));
        }
        fmt::print("{}\n", output.str());
    }

  template <typename OArchive> void print(const QueryRequest &qinfo) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Request", qinfo));
        }
        fmt::print("{}\n", output.str());
    }

    template <typename OArchive> void print(const HashTable &info) {
        std::ostringstream output;
        {
            OArchive oar(output);
            oar(cereal::make_nvp("Hash table", info));
        }
        fmt::print("{}\n", output.str());
    }
}
#endif
