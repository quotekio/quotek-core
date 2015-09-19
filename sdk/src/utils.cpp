/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "utils.hpp"

namespace quotek {

  namespace core {

    namespace utils {

      std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
          std::stringstream ss(s);
          std::string item;
          while (std::getline(ss, item, delim)) {
              elems.push_back(item);
          }
          return elems;
      }

      std::vector<std::string> split(const std::string &s, char delim) {
          std::vector<std::string> elems;
          split(s, delim, elems);
          return elems;
      }      
    }
  }
}
