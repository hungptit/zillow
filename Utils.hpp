#ifndef Utils_hpp
#define Utils_hpp

#include <string>
#include "Zillow.hpp"

namespace zillow {
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

}

#endif
