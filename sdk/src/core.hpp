/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
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

    /** Just a bit of fun */
    float glooper(float gdp,
    	          float gdp_per_capita,
    	          float growth_rate,
    	          float key_landing_rate,
    	          float qe_size,
    	          float inflation,
    	          float cpi,
    	          float balance_sheet,
    	          float median_income,
    	          float average_income,
    	          float unemployment_rate,
    	          int season,
    	          int weather,
    	          float moon_distance,
    	          float sun_distance,
    	          int captains_age
    	        ) {

    	return 0;

    }

    


  }
}

#endif
