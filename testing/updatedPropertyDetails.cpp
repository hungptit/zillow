#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "fmt/format.h"
#include "zillow/ZillowWebCrawler.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help,h", "This command will query information for a house using zillow DeepSearch API.")
      ("verbose,v", "Display verbose information.")
      ("zpid,z", po::value<size_t>(), "Zillow house ID")
      ("zwpid,w", po::value<std::string>(), "Zillow web service ID");
    // clang-format on

    po::positional_options_description p;
    p.add("zpid", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Parse input arguments
    if (vm.count("help")) {
        std::cout << desc;
        return false;
    }

    bool verbose = vm.count("verbose");

    size_t zpid;
    if (vm.count("zpid")) {
        zpid = vm["zpid"].as<size_t>();
    } else {
        std::cerr << "You need to provide zpid of a house\n";
        return 1;
    }

    std::string zwpid = zillow::ZWSID::ID1;
    if (vm.count("zwpid")) {
        zwpid = vm["zwpid"].as<std::string>();
    }

    std::string queryCmd = zillow::generateUpdatedPropertyDetailsQuery(zwpid, zpid);
    if (verbose) {
        fmt::print("Query link: {}\n", queryCmd);
    }

    std::stringstream output;
    auto status = zillow::query(queryCmd, output);
    if (!status) {
        fmt::print("Could not query this link: {}\n", queryCmd);
        return 1;
    }

    if (verbose) {
        fmt::print("{}\n", output.str());
    }

    // Parse the query results.
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults = doc.load(output);
    if (!parseResults) {
        fmt::print("status: {}\n", parseResults.status);
        fmt::print("description: {}\n", parseResults.description());
    }

    auto response = zillow::parseUpdatedPropertyDetails(
        doc.child("UpdatedPropertyDetails:updatedPropertyDetails").child("response"));
    zillow::print<cereal::JSONOutputArchive>(response);

    return 0;
}
