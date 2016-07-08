#include "gtest/gtest.h"
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "utils/TemporaryDirectory.hpp"
#include "utils/Timer.hpp"

#include "zillow/Constraints.hpp"
#include "zillow/Hash.hpp"
#include "zillow/Operators.hpp"
#include "zillow/Zillow.hpp"
#include "zillow/ZillowWebCrawler.hpp"

#include <unordered_set>

TEST(EdgeData, Positive) {
    std::vector<zillow::EdgeData> data = {
        zillow::EdgeData{1, 2, 3.0}, zillow::EdgeData{2, 3, 3.0}, zillow::EdgeData{2, 3, 3.0},
        zillow::EdgeData{2, 3, 4.0}};
    std::sort(data.begin(), data.end(), zillow::Greater<zillow::EdgeData>());
    std::unordered_set<zillow::EdgeData> aSet(data.begin(), data.end());

    EXPECT_TRUE(data.size() == 4);
    EXPECT_TRUE(aSet.size() == 3);

    // Display the output
    std::ostringstream output;
    {
        using OArchive = cereal::JSONOutputArchive;
        OArchive oar(output);
        oar(cereal::make_nvp("Vector", data), cereal::make_nvp("unordered_set", aSet));
    }
    fmt::print("Results: \n{}\n", output.str());
}

TEST(Constraints, Positive) {
    std::vector<std::string> cities{"Needham", "Newton", "Enfield"};
    std::vector<std::string> states{"MA", "CT"};
    zillow::BasicConstraints constraints(cities, states);
    {
        zillow::Address anAddress("", "02492", "Needham", "MA", 0.0, 0.0);
        EXPECT_TRUE(constraints.isValid(anAddress));
    }

    {
        zillow::Address anAddress("", "0", "Enfield", "CT", 0.0, 0.0);
        EXPECT_TRUE(constraints.isValid(anAddress));
    }

    {
        zillow::Address anAddress("", "0", "Enfield1", "CT", 0.0, 0.0);
        EXPECT_FALSE(constraints.isValid(anAddress));
    }

    {
        // TODO: Will need to change interface to exclude this case.
        zillow::Address anAddress("", "0", "Needham", "CT", 0.0, 0.0);
        EXPECT_TRUE(constraints.isValid(anAddress));
    }
}
