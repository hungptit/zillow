#include "Poco/Exception.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"

#include "fmt/format.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

std::string doRequest(Poco::Net::HTTPClientSession &session,
                      Poco::Net::HTTPRequest &request,
                      Poco::Net::HTTPResponse &response) {
    session.sendRequest(request);
    std::istream &rs = session.receiveResponse(response);
    std::stringstream output;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        StreamCopier::copyStream(rs, output);
    } else {
        std::cout << response.getStatus() << " " << response.getReason()
                  << std::endl;
    }

    return output.str();
}

void strrep(std::string &s, char oldChar, char newChar) {
    const size_t N = s.size();
    for (size_t idx = 0; idx < N; ++idx) {
        if (s[idx] == oldChar) {
            s[idx] = newChar;
        }
    }
}

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

    strrep(street, ' ', '+');
    strrep(city, ' ', '+');

    fmt::MemoryWriter output;
    output << "http://www.zillow.com/webservice/"
           << "GetDeepSearchResults.htm?zws-id=X1-ZWz1f8wdb88lxn_1y8f9&"
           << "address=" << street << "&citystatezip=" << city << "%2C+"
           << state;

    if (verbose) {
        fmt::print("Query link: {}\n", output.str());
    }

    std::string aLink(output.str());
    URI uri(aLink);
    std::string path(uri.getPathAndQuery());
    HTTPClientSession session(uri.getHost(), uri.getPort());
    HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
    HTTPResponse response;
    auto results = doRequest(session, request, response);
    if (results.empty()) {
        std::cerr << "Cannot query this link: " << path << "\n";
    }
    fmt::print("Output: {}\n", results);

    return 0;
}
