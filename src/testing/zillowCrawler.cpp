#include "Poco/Exception.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Net/HTTPCredentials.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "fmt/format.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

enum HTTPStatus {OK = 200, BAD_REQUEST=400};

std::string doRequest(Poco::Net::HTTPClientSession &session,
               Poco::Net::HTTPRequest &request,
               Poco::Net::HTTPResponse &response) {
    session.sendRequest(request);
    std::istream &rs = session.receiveResponse(response);

    const int status = response.getStatus();
    if (status == OK) {
    } else {
      std::cout << status << " " << response.getReason()
                << std::endl;
    }

    std::stringstream output;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        StreamCopier::copyStream(rs, output);
    }   
    
    return output.str();
}

int main(int argc, char **argv) {
    std::string aLink("http://www.zillow.com/webservice/"
                      "GetDeepSearchResults.htm?zws-id=X1-ZWz1f8wdb88lxn_1y8f9&"
                      "address=302+Warrent+St&citystatezip=Needham%2C+MA");
    URI uri(aLink);
    std::string path(uri.getPathAndQuery());
    
    try {
        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        HTTPResponse response;
        auto results = doRequest(session, request, response);
        if (results.empty()) {
            std::cerr << "Cannot query this link: " << path << "\n";
        }
        fmt::print("Output: {}\n", results);
    } catch (Exception &exc) {
        std::cerr << exc.displayText() << std::endl;
        return 1;
    }
    
    return 0;
}
