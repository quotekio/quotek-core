/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef CORE_HPP
#define CORE_HPP

#include "constants.hpp"
#include <string>

namespace quotek {

  namespace core {
    
    /** Returns the version of the SDK */
    std::string version() {
      return std::string(SDK_VERSION);
    }

  }
}

#endif
