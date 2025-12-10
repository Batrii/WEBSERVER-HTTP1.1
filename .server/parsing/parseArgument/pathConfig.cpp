#include <parseArgument.hpp>

// std::string filepath(argument);
// size_t dotPos = filepath.find_last_of('.');
// if (dotPos == std::string::npos || filepath.substr(dotPos) != ".json")
//   throw std::runtime_error("Configuration file must have `.json` extension");

void pathConfig(char* file, clock_tt startClock) {

  boost::property_tree::ptree jsonConfigFile;
  read_json(file, jsonConfigFile);

  // // Iterate through the servers array
  // boost::property_tree::ptree servers = jsonConfigFile.get_child("servers");
  // for (boost::property_tree::ptree::iterator it = servers.begin(); it != servers.end(); ++it) {
  //   // Get the name from each server object
  //   std::string name = it->second.get<std::string>("name");
  //   std::cout << "Server name: " << name << std::endl;
  // }

  (void)startClock;
  throw std::runtime_error("pathConfig function is not implemented yet");
}