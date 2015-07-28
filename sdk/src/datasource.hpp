/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef DATASOURCE_HPP
#define DATASOURCE_HPP

#include <string>

#include "record.hpp"

namespace quotek {

  namespace datasource {
     
    /**
     * datasource is a meta class meant to retrieve data from multiple data sources.
     */   
    class datasource {

      public:

      	std::string source;
        
    };

    class text_datasource: public datasource {

      public:

      	std::string source;
        
    };

    class value_datasource: public datasource {

    };

  }
}

#endif
