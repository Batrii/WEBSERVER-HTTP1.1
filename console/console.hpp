#pragma once
#include <iostream>

class console {
  private:
    int const _PORT;
    std::string const _NETWORK;
    std::string const _NAME;
    std::string const _VERSION;
  public:
    console(
      int port = 3000,
      std::string network = "127.0.0.1",
      std::string name = "www",
      std::string version = "0.1.0"
    );
    void init(void) const throw();
    void success(char const*) const throw();
    void issue(char const*) const throw();
    void info(char const*) const throw();
    void warning(char const*) const throw();
    void log(char const*) const throw();
    void GET(char const*, int, char const*) const throw();
    void POST(char const*, int, char const*) const throw();
    void DELETE(char const*, int, char const*) const throw();
};