#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "fmt/format.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "zillow/Serialization.hpp"
#include "zillow/XMLParser.hpp"
#include "zillow/ZillowWebCrawler.hpp"

int main(int argc, char **argv) {
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");

    // clang-format off
    desc.add_options()
      ("help,h", "This command will query information for a house using zillow DeepSearch API.")
      ("verbose,v", "Display verbose information.")
      ("output-to-xml,x", "Output a raw crawled data to a XML file")
      ("street,a", po::value<std::string>(), "Street")
      ("city,c", po::value<std::string>(),"City")
      ("state,s", po::value<std::string>(), "State")
      ("zwpid,w", po::value<std::string>(), "Zillow web ID");;
    // clang-format on

    po::positional_options_description p;
    p.add("street", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Parse input arguments
    if (vm.count("help")) {
        std::cout << desc;
        return false;
    }

    bool verbose = vm.count("verbose");

    std::string street;
    if (vm.count("street")) {
        street = vm["street"].as<std::string>();
    } else {
        std::cerr << "You need to provide street address\n";
        return 1;
    }

    std::string city;
    if (vm.count("city")) {
        city = vm["city"].as<std::string>();
    } else {
        std::cerr << "You need to provide city.\n";
        return 1;
    }

    std::string state;
    if (vm.count("state")) {
        state = vm["state"].as<std::string>();
    } else {
        std::cerr << "You need to provide state.\n";
        return 1;
    }

    std::string zwpid = zillow::ZWSID::ID1;
    if (vm.count("zwpid")) {
        zwpid = vm["zwpid"].as<std::string>();
    }

    fmt::print("{}\n", zwpid);

    std::string queryCmd = zillow::generateDeepSearchQuery(
        zwpid, zillow::Address(street, "0", city, state, 0.0, 0.0));
    if (verbose) {
        fmt::print("Query link: {}\n", queryCmd);
    }

    std::stringstream output;
    bool status = zillow::query(queryCmd, output);

    if (vm.count("output-to-xml")) {
        zillow::writeTextFile(output, "deepSearchResults.xml");
    }

    if (!status) {
        fmt::print("Cannot query this link: {}", queryCmd);
    } else {
        pugi::xml_document doc;
        pugi::xml_parse_result parseResults = doc.load(output);
        if (!parseResults) {
            fmt::print("status: {}\n", parseResults.status);
            fmt::print("description: {}\n", parseResults.description());
        }

        {
            auto request = zillow::parseDeepSearchResultsRequest(
                doc.child("SearchResults:searchresults").child("request"));

            auto message =
                zillow::parseMessage(doc.child("SearchResults:searchresults").child("message"));
            zillow::print<cereal::JSONOutputArchive>(message);

            auto response =
                zillow::parseDeepSearchResultsResponse(doc.child("SearchResults:searchresults")
                                                           .child("response")
                                                           .child("results")
                                                           .child("result"));

            zillow::print<cereal::JSONOutputArchive>(response);
        }
    }

    return 0;
}
