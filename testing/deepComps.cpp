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
      ("output-to-xml,x", "Output a raw crawled data to a XML file")
      ("zpid,z", po::value<size_t>(), "Zillow house ID")
      ("count,c", po::value<size_t>(), "")
      ("zwpid,w", po::value<std::string>(), "Zillow web ID");
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

    size_t count = 25;
    if (vm.count("count")) {
        count = vm["count"].as<size_t>();
    }

    std::string zwpid = zillow::ZWSID::ID1;
    if (vm.count("zwpid")) {
        zwpid = vm["zwpid"].as<std::string>();
    }

    std::string queryCmd = zillow::generateDeepCompsQuery(zwpid, zpid, count);

    if (verbose) {
        fmt::print("Query link: {}\n", queryCmd);
    }
    std::stringstream output;
    auto queryResults = zillow::query(queryCmd, output);

    if (vm.count("output-to-xml")) {
        zillow::writeTextFile(output, "deepCompsResults.xml");
    }

    if (verbose) {
        fmt::print("{}\n", output.str());
    }

    if (!queryResults) {
        fmt::print("Could query this link: {}\n", queryCmd);
    }

    // Parse the query results.
    pugi::xml_document doc;
    pugi::xml_parse_result parseResults = doc.load(output);
    if (!parseResults) {
        fmt::print("status: {}\n", parseResults.status);
        fmt::print("description: {}\n", parseResults.description());
    }

    auto response = zillow::parseDeepCompsResponse(
        doc.child("Comps:comps").child("response").child("properties"));

    // Display all comparable homes
    zillow::print<cereal::JSONOutputArchive>(std::get<0>(response));

    // Display edge information.
    // zillow::print<cereal::JSONOutputArchive>(std::get<1>(response));
    
    return 0;
}
