#include "gtest/gtest.h"
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "utils/TemporaryDirectory.hpp"
#include "utils/Timer.hpp"

#include "zillow/Zillow.hpp"
#include "zillow/ZillowWebCrawler.hpp"

TEST(UpdatedPropertyDetails, Positive) {
  
}

TEST(Constraints, Positive) {
    std::vector<std::string> cities{"Needham", "Newton", "Enfield"};
    std::vector<std::string> states{"MA", "CT"};
    zillow::BasicConstraints constraints(cities, states);
    {
        zillow::Address anAddress("", 0, "Needham", "MA", 0.0, 0.0);
        EXPECT_TRUE(constraints.isValid(anAddress));
    }

    {
        zillow::Address anAddress("", 0, "Enfield", "CT", 0.0, 0.0);
        EXPECT_TRUE(constraints.isValid(anAddress));
    }

    {
        zillow::Address anAddress("", 0, "Enfield1", "CT", 0.0, 0.0);
        EXPECT_FALSE(constraints.isValid(anAddress));
    }

    {
        // TODO: Will need to change interface to exclude this case.
        zillow::Address anAddress("", 0, "Needham", "CT", 0.0, 0.0);
        EXPECT_TRUE(constraints.isValid(anAddress));
    }
}
