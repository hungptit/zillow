#ifndef Hash_hpp
#define Hash_hpp

#include "Zillow.hpp"

// Define the hash objects for some data structures.
namespace std {
    template <> struct hash<zillow::EdgeData> {
        typedef zillow::EdgeData argument_type;
        typedef std::size_t result_type;
        result_type operator()(const argument_type &aKey) const {
            result_type const h1(std::hash<zillow::IDType>()(aKey.SrcID));
            result_type const h2(std::hash<zillow::IDType>()(aKey.DstID));
            result_type const h3(std::hash<zillow::IDType>()(aKey.Score));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    template <> struct hash<zillow::Address> {
        typedef zillow::Address argument_type;
        typedef std::size_t result_type;
        result_type operator()(const argument_type &aKey) const {
            result_type const h1(std::hash<std::string>()(aKey.Street));
            result_type const h2(std::hash<std::string>()(aKey.City));
            result_type const h3(std::hash<std::string>()(aKey.State));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

#endif
