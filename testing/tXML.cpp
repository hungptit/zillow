#include "fmt/format.h"
#include "pugixml.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <vector>

#include "boost/filesystem.hpp"

#include "zillow/Database.hpp"
#include "zillow/Serialization.hpp"
#include "zillow/Serialization.hpp"
#include "zillow/XMLParser.hpp"

#include <iomanip>
#include <iostream>
#include <locale>
#include <locale>
#include <sstream>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

TEST(parseUpdatedPropertyDetailsResults, Positive) {
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults =
        doc.load_file("updatedPropertyDetails.xml", pugi::parse_full);

    if (parseResults) {
        fmt::print("status: {}\n", parseResults.status);
        fmt::print("description: {}\n", parseResults.description());
    }

    auto response = zillow::parseUpdatedPropertyDetails(
        doc.child("UpdatedPropertyDetails:updatedPropertyDetails").child("response"));
    zillow::print<cereal::JSONOutputArchive>(response);
    EXPECT_TRUE(response.zpid == 57477487);

    zillow::PageViewCount expectedCount{{3191, 4191}};
    EXPECT_EQ(response.pageViewCount, expectedCount);

    std::string expectedAppliances(
        "Dryer, Dishwasher, Refrigerator, Range / Oven, Garbage disposal, Washer");
    EXPECT_EQ(response.editedFacts.Appliances, expectedAppliances);
}

TEST(parseDeepSearchResults, Positive) {
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults =
        doc.load_file("deepSearchResults.xml", pugi::parse_full);

    if (parseResults) {
        fmt::print("status: {}\n", parseResults.status);
        fmt::print("description: {}\n", parseResults.description());
    }

    auto request = zillow::parseDeepSearchResultsRequest(
        doc.child("SearchResults:searchresults").child("request"));
    fmt::print("request: \n\taddress: {0}\n\tcitystatezip : {1}\n", std::get<0>(request),
               std::get<1>(request));

    auto message =
        zillow::parseMessage(doc.child("SearchResults:searchresults").child("message"));
    zillow::print<cereal::JSONOutputArchive>(message);

    auto response =
        zillow::parseDeepSearchResultsResponse(doc.child("SearchResults:searchresults")
                                                   .child("response")
                                                   .child("results")
                                                   .child("result"));
    zillow::print<cereal::JSONOutputArchive>(response);
    zillow::print<cereal::XMLOutputArchive>(response);

    EXPECT_TRUE(response.zpid == 57474939);
    EXPECT_EQ(response.info.HouseAddress.Street, "302 Warren St");

    {
        zillow::NodeParser parser(doc);
        auto const &data = parser.getData();
        zillow::print<cereal::JSONOutputArchive>(data);
        fmt::print("TimeStamp: {}\n", parser.getTimeStamp());
        std::string timeStamp = parser.getTimeStamp();
        std::string expectedTimeStamp("Thu Jun 16 07:21:24 PDT 2016  ");
        EXPECT_EQ(expectedTimeStamp, timeStamp);
        // auto t = zillow::to_tm(parser.getTimeStamp());
        // auto results = std::put_time(&t, "%c");
        // fmt::print("Parsed time {}", std::string(results));
        // // std::cout << "Parsed time: " << std::put_time(&t, "%c") << "\n";
    }
}

TEST(parseDeepCompsResults, Positive) {
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults = doc.load_file("deepCompsResults.xml");

    if (parseResults) {
        fmt::print("status: {}\n", parseResults.status);
        fmt::print("description: {}\n", parseResults.description());
    }

    auto request = zillow::parseDeepCompsRequest(doc.child("Comps:comps").child("request"));
    fmt::print("request: \n\tzpid: {0}\n\tcount : {1}\n", std::get<0>(request),
               std::get<1>(request));

    auto message = zillow::parseMessage(doc.child("Comps:comps").child("message"));
    zillow::print<cereal::JSONOutputArchive>(message);

    // zillow::dfs(rootNode.child("response").child("properties"), "");
    auto results = zillow::parseDeepCompsResponse(
        doc.child("Comps:comps").child("response").child("properties"));

    // const zillow::DeepSearchResults &principal = std::get<0>(results);
    auto const &deepComps = std::get<0>(results);
    auto const &edges = std::get<1>(results);

    // zillow::print<cereal::JSONOutputArchive>(deepComps);
    // zillow::print<cereal::JSONOutputArchive>(edges);

    EXPECT_TRUE(deepComps.size() == 25);
    EXPECT_TRUE(edges.size() == 25);

    // Write information to the database
    std::string dataFile("test_database.db");
    
    boost::filesystem::path aPath(dataFile);
    if (boost::filesystem::exists(aPath)) {
        boost::filesystem::remove(aPath);
    }
    
    zillow::writeToSQLite(dataFile, deepComps, edges);
}
