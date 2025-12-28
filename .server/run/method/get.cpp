#include <server.hpp>
#include <request.hpp>
#include <console.hpp>
#include <status.hpp>
#include <type.hpp>
#include <time.hpp>
#include <response.hpp>
#include <path.hpp>
#include <error.hpp>
#include <fstream>
#include <sys/socket.h>
#include <permission.hpp>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <dirent.h>

void methodGet(int client, request& req, ctr& currentServer, long long startRequestTime) {

  // find matching route at config file
  rt* route = NULL;
  for (std::size_t i = 0; i < currentServer.length(); i++) {
    if (path::check(currentServer.route(i).path(), req.getPath())) {
      route = &currentServer.route(i);
      break;
    }
  }

  std::string sourcePathToHandle;

  if (!route) {
    // absolute path
    sourcePathToHandle = currentServer.root() + req.getPath();
    // Automatic index resolution for GET `server.index()`
    
    // check if file exists
    struct stat fileStat;
    if (stat(sourcePathToHandle.c_str(), &fileStat) != 0 || permission::check(sourcePathToHandle)) {
      // 404 not found
      std::map<std::string, std::string> Theaders;
      Theaders["Content-Type"] = "text/html";
      response(client, startRequestTime, 404, Theaders, "", req, currentServer).sendResponse();
      return;
    }

    // check if it's a directory
    if (S_ISDIR(fileStat.st_mode))
      sourcePathToHandle = sourcePathToHandle + "/" + currentServer.index();
 
  } else {
    sourcePathToHandle = route->source();

    // 405 method not allowed
    if (
      route->method(0) != "GET" &&
      (route->length() > 1 && route->method(1) != "GET") &&
      (route->length() > 2 && route->method(2) != "GET")
    ) {
      std::map<std::string, std::string> Theaders;
      Theaders["Allow"] = "GET, POST, DELETE";
      Theaders["Content-Type"] = "text/html";
      response(client, startRequestTime, 405, Theaders, "", req, currentServer).sendResponse();
      return;
    }
  }

  // handle redirections
  if (route && !route->redirect().empty()) {
    std::map<std::string, std::string> Theaders;
    Theaders["Location"] = route->redirect();
    response(client, startRequestTime, (route->redirect().find("http") == 0 ? 302 : 301), Theaders, "", req, currentServer).sendResponse();
    return;
  }

  // handle dictlist
  if (route && route->dictlist()) {
    return;
  }

  // handle cgi execution
  if (route && route->cgiScript().empty()) {
    ;
  }

  std::map<std::string, std::string> Theaders;
  Theaders["Content-Type"] = type::get(sourcePathToHandle);
  response(client, startRequestTime, 1337, Theaders, "", req, currentServer).sendGETchunks(sourcePathToHandle);
  return;
}