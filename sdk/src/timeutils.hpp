/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef TIMEUTILS_HPP
#define TIMEUTILS_HPP

#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include <time.h>

namespace quotek {

  namespace core {
      
    namespace time {

        /**
         * is_time tells if a time string matches with the epoch timestamp provided
         * as an argument.
         */
        bool is_time(std::string time_string, 
                     long timestamp);

        /**
         * trade_hours tells if the market is open or not according to open and close hours
         * provided as time strings.
         */
        bool trade_hours(std::string time_open, 
                         std::string time_close, 
                         long timestamp);

        /** week_day returns the long day name according to a provided epoch timestamp.
         */
        std::string week_day(long timestamp);

        /** month_day provids the number of the day of the month according to provided timestamp. */
        int month_day(long timestamp);

    }
  }
}

#endif
