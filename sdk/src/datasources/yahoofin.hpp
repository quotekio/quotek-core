/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef YAHOOFIN_HPP
#define YAHOOFIN_HPP

#include <string>
#include "../http.hpp"
#include "../datasource.hpp"

namespace quotek {

  namespace datasource {
   
    /** yahoofin is a class which allows to query the Yahoo! Finance API service to search for 
    
    */

    class yahoofin: public datasource_value {

      public:

      	yahoofin();
      	~yahoofin();

    };


  }
}

#endif
