#ifndef ZillowWebCrawler_hpp_
#define ZillowWebCrawler_hpp_

#include "Poco/Exception.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

namespace {
    std::string doRequest(Poco::Net::HTTPClientSession &session,
                          Poco::Net::HTTPRequest &request,
                          Poco::Net::HTTPResponse &response) {
        session.sendRequest(request);
        std::istream &rs = session.receiveResponse(response);
        std::stringstream output;
        if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
            Poco::StreamCopier::copyStream(rs, output);
        } else {
            // fmt::print("Cannot query this link: {}", path);
            fmt::print("Status: {0}\nReason: {1}\n", response.getStatus(),
                       response.getReason());
        }

        return output.str();
    }
}

namespace zillow {
    std::string query(const std::string &aLink) {
        Poco::URI uri(aLink);
        std::string path(uri.getPathAndQuery());
        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path,
                                       Poco::Net::HTTPMessage::HTTP_1_1);
        Poco::Net::HTTPResponse response;
        return doRequest(session, request, response);
    }

    void strrep(std::string &s, char oldChar, char newChar) {
        const size_t N = s.size();
        for (size_t idx = 0; idx < N; ++idx) {
            if (s[idx] == oldChar) {
                s[idx] = newChar;
            }
        }
    }
}
#endif
