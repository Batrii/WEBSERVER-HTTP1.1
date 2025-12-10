#pragma once
#include <iostream>
#include <vector>

class ServersInfo {
  private:
    class route {
      private:
        std::string path;
        std::string source;
        std::vector<std::string> method[];
    };
    class server {
      private:
        unsigned int port;
        std::string name;
        std::string version;
        std::string root;
        std::vector<route> routes[];
    };

    std::vector<server> servers[];
  public:
    inline std::size_t length() const throw();
};

extern ServersInfo serversInfo;
/* global object declaration, see ./serversInfo.cpp for actual definition */