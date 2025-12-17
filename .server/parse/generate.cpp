#include <dirent.h>
#include <netdb.h>
#include <server.hpp>
#include <sstream>
#include <vector>

static void createRoutes(std::string& fullPath, ctr& currentServer) {
  (void)currentServer;
  std::cout << fullPath << std::endl;
}

std::size_t defaultPort = 3000;
static void createServer(std::string name) {
  ctr& currentServer = server.create();

  // set name
  currentServer.name() = name;
  // set default values
  currentServer.port() = defaultPort;
  defaultPort++;
  currentServer.version() = "0.1.0";
  currentServer.notfound() = ".server/.build/not-found.html";
  currentServer.servererror() = ".server/.build/server-error.html";
  currentServer.log() = ".server/.log/" + currentServer.name() + "/" + currentServer.name() + ".log";
  currentServer.bodylimit() = 1048576;
  currentServer.timeout() = 30000;
  currentServer.index() = "./index.html";
  currentServer.root() = "app/" + currentServer.name() + "/";
  currentServer.uploaddir() = currentServer.root() + "/uploads";

  // read directory and make routes
  std::string fullPath = "app/" + name + "/";
  createRoutes(fullPath, currentServer);
}

void generate(void) {

  // open app directory
  DIR* appDir = opendir("app");
  if (!appDir)
    throw std::runtime_error("could not open app directory");

  std::size_t appSize = 0;
  std::vector<std::string> appArray;
  struct dirent* contentTemp;

  // read each time directory
  while ((contentTemp = readdir(appDir)) != NULL) {
    std::string name = contentTemp->d_name;
    if (name == "." || name == "..")
      continue;
    if (contentTemp->d_type == DT_DIR) {
      // this is a directory
      appSize++;
      appArray.push_back(name);
    }
    else if (contentTemp->d_type == DT_REG) // this is a file
      throw std::runtime_error("app directory cannot contain files");
  }

  // check number of directories
  if (appSize <= 0)
    throw std::runtime_error("no web applications found in app directory");

  // loop each folder and create server for each application
  for (std::size_t i = 0; i < appArray.size(); i++) {
    createServer(appArray[i]);
  }

  closedir(appDir);
}