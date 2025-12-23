#include <server.hpp>
#include <request.hpp>
#include <console.hpp>
#include <status.hpp>
#include <time.hpp>
#include <sys/socket.h>
#include <response.hpp>
class urlencoder {
  private:
    std::map<std::string, char> _charMap;
  public:
    urlencoder();
};

void methodPost(int client, request& req, ctr& currentServer, long long startRequestTime) {
  if (req.getBody().empty()) {
    std::map<std::string, std::string> headers;
    std::string body = "";
    response res(client, startRequestTime, 400, headers, body, req);
    res.sendResponse();
    return;
  }

  if (req.getHeaders().find("Content-Length") != req.getHeaders().end() && req.getHeaders().at("Content-Length") != "0") {
    std::string contentType = req.getHeaders().at("Content-Type");
    if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
    {}
    else if (contentType.find("multipart/form-data") != std::string::npos)
    {}
    else if (contentType.find("text/plain") != std::string::npos)
    {
      std::string contentstored = req.getBody();
      std::map<std::string, std::string> headers;
      std::string body = "Data received:\n" + contentstored;
      response(client, startRequestTime, 200, headers, body, req).sendResponse();
      return;
    }
    else if (contentType.find("application/json") != std::string::npos)
    {}
    else {
      std::map<std::string, std::string> headers;
      std::string body = "";
      response(client, startRequestTime, 415, headers, body, req).sendResponse();
      return;
    }
  }
  else {
    std::map<std::string, std::string> headers;
    std::string body = "";
    response res(client, startRequestTime, 411, headers, body, req);
    res.sendResponse();
    return;
  }
}