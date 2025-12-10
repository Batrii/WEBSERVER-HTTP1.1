#include <serversInfo.hpp>

ServersInfo serversInfo; /* actual definition */

inline std::size_t ServersInfo::length() const throw() {
  return this->servers->size();
}