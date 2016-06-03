#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "fmt/format.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "zillow/ZillowWebCrawler.hpp"

int main(int argc, char **argv) {
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help,h", "This command will query information for a house using zillow DeepSearch API.")
      ("verbose,v", "Display verbose information.")
      ("zpid,z", po::value<size_t>(), "Zillow house ID")
      ("count,c", po::value<size_t>(), "")
      ("zwpid,w", po::value<std::string>(), "Zillow house ID");
    // clang-format on

    po::positional_options_description p;
    p.add("zpid", -1);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).options(desc).positional(p).run(),
        vm);
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
      count= vm["count"].as<size_t>();
    }

    std::string zwpid = "X1-ZWz1f8wdb88lxn_1y8f9";
    if (vm.count("zwpid")) {
      zwpid = vm["zwpid"].as<std::string>();
    } 

    fmt::MemoryWriter output;
    output << "http://www.zillow.com/webservice/"
           << "GetDeepComps.htm?zws-id=X1-ZWz1f8wdb88lxn_1y8f9&"
           << "&zpid=" << zpid
           << "&count=" << count;
    
    if (verbose) {
        fmt::print("Query link: {}\n", output.str());
    }

    auto results = zillow::query(output.str());    
    fmt::print("Output: {}\n", results);

    return 0;
}