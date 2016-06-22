#ifndef Operators_hpp
#define Operators_hpp

#include "Zillow.hpp"

namespace zillow {
    bool operator==(const EdgeData &lhs, const EdgeData &rhs) {
        return lhs.SrcID == rhs.SrcID && lhs.DstID == rhs.DstID;
    }

    bool operator==(const Address &lhs, const Address &rhs) {
        return lhs.Street == rhs.Street && lhs.ZipCode == rhs.ZipCode && lhs.City == rhs.City &&
               lhs.State == rhs.State && lhs.ZipCode == rhs.ZipCode;
    }

    template<typename T> struct Greater;
    template<typename T> struct Less;

    template <> struct Greater<EdgeData> {
        bool operator()(EdgeData a, EdgeData b) {
            return (a.SrcID > b.SrcID) || ((a.SrcID == b.SrcID) && (a.DstID > b.DstID));
        }
    };

    template <> struct Less<EdgeData> {
        bool operator()(EdgeData a, EdgeData b) {
            return (a.SrcID < b.SrcID) || ((a.SrcID == b.SrcID) && (a.DstID < b.DstID));
        }
    };
}

#endif
