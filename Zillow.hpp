#ifndef Zillow_hpp_
#define Zillow_hpp_

#include "Operators.hpp"
#include "DataStructures.hpp"
#include "Constraints.hpp"
#include "Hash.hpp"
#include "Utils.hpp"
#include "Serialization.hpp"

namespace zillow {
    struct ZWSID {
        static const std::string ID1;
        static const std::string ID2;
    };

    const std::string ZWSID::ID1 = "X1-ZWz1fbpbq5pogb_3wguv";
    const std::string ZWSID::ID2 = "X1-ZWz1f8wdb88lxn_1y8f9";
}
#endif
