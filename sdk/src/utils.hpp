/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

namespace quotek {

  namespace core {
      
    /**
     * This namespace contains conveniency and other useful functions for everyday C++ development.
     */

    namespace utils {

      std::vector<std::string> &split(const std::string &s, 
                               char delim, 
                               std::vector<std::string> &elems);

      std::vector<std::string> split(const std::string &s, 
                                     char delim);

      std::string &ltrim(std::string &s);
      std::string &rtrim(std::string &s);
      std::string &trim(std::string &s);


    }
  }
}

#endif
