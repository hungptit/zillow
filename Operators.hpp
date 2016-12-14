#ifndef Operators_hpp
#define Operators_hpp

#include "DataStructures.hpp"

namespace zillow {

    template <typename T> struct Greater;
    template <typename T> struct Less;

    template <> struct Greater<EdgeData> {
        bool operator()(EdgeData lhs, EdgeData rhs) {
            return std::tie(lhs.SrcID, lhs.DstID, lhs.Weight) >
                   std::tie(rhs.SrcID, rhs.DstID, rhs.Weight);
        }
    };

    template <> struct Less<EdgeData> {
        bool operator()(EdgeData lhs, EdgeData rhs) {
            return std::tie(lhs.SrcID, lhs.DstID, lhs.Weight) >
                   std::tie(rhs.SrcID, rhs.DstID, rhs.Weight);
        }
    };
}

#endif
