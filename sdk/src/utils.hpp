/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#ifdef _WIN64
  #include <functionnal>
  #include <cctype>
  #include <locale>
#endif

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
      
      unsigned int randr(unsigned int min, unsigned int max);


    }
  }
}

#endif
