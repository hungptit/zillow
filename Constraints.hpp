#ifndef Constraints_hpp
#define Constraints_hpp

#include <string>
#include "Zillow.hpp"
#include "fmt/format.h"

namespace zillow {
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
