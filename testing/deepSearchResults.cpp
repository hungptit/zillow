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
        ("street,a", po::value<std::string>(), "Street")
        ("city,c", po::value<std::string>(),"City")
        ("state,s", po::value<std::string>(), "State");
    // clang-format on

    po::positional_options_description p;
    p.add("street", -1);

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

    zillow::strrep(street, ' ', '+');
    zillow::strrep(city, ' ', '+');

    fmt::MemoryWriter output;
    output << "http://www.zillow.com/webservice/"
           << "GetDeepSearchResults.htm?zws-id=X1-ZWz1f8wdb88lxn_1y8f9&"
           << "address=" << street << "&citystatezip=" << city << "%2C+"
           << state;

    if (verbose) {
        fmt::print("Query link: {}\n", output.str());
    }

    std::string results = zillow::query(output.str());
    if (results.empty()) {
      fmt::print("Cannot query this link: {}", output.str());
    } else {
      fmt::print("Output: {}\n", results);
    }
    

    return 0;
}
