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
        ("street,a", po::value<std::string>(), "Street")
        ("city,c", po::value<std::string>(),"City")
        ("state,s", po::value<std::string>(), "State")
        ("max-house,m", po::value<size_t>(), "Street")
        ("desired-cities,t", po::value<std::vector<std::string>>(), "Desired cities.")
        ("desired-states,b", po::value<std::vector<std::string>>(), "Desired states.")
        ("zwpid,w", po::value<std::string>(), "Zillow web ID")
        ("database,d", po::value<std::string>(), "Database prefix.");;
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

    size_t max_houses = 100;
    if (vm.count("max-house")) {
        max_houses = vm["max-house"].as<size_t>();
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

    std::string zwpid =
        (vm.count("zwpid")) ? (vm["zwpid"].as<std::string>()) : zillow::ZWSID::ID2;
    // if (vm.count("zwpid")) {
    //     zwpid = vm["zwpid"].as<std::string>();
    // }

    std::vector<std::string> desiredCities;
    if (vm.count("desired-cities")) {
        desiredCities = vm["desired-cities"].as<std::vector<std::string>>();
    } else {
        desiredCities.emplace_back(city);
    }

    std::vector<std::string> desiredStates;
    if (vm.count("desired-states")) {
        desiredStates = vm["desired-state"].as<std::vector<std::string>>();
    } else {
        desiredStates.emplace_back(state);
    }

    std::string database =
        (vm.count("database")) ? vm["database"].as<std::string>() : "database";

    if (verbose) {
        fmt::print("Query info:\n\tStreet: {0}\n\tCity: {1}\n\tState: {2}\n\tmax_houses: "
                   "{3}\n\tDatabase: {4}\n",
                   street, city, state, max_houses, database);

        fmt::print("\tzwpid: {}\n", zwpid);

        fmt::print("\tDesired cities: ");
        for (auto const &item : desiredCities) {
            fmt::print("{} ", item);
        }
        fmt::print("\n");

        fmt::print("\tDesired states: ");
        for (auto const &item : desiredStates) {
            fmt::print("{}", item);
        }
        fmt::print("\n");
    }

    {
        zillow::BasicConstraints constraints(desiredCities, desiredStates);
        zillow::Crawler crawler(zwpid, max_houses, database);
        crawler.exec(zillow::Address(street, 0, city, state, 0.0, 0.0), 25, constraints);
        crawler.save();
    }

    return 0;
}
