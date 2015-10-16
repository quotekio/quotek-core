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
#include <ctime>
#include <time.h>

#ifdef _WIN64
#define sscanf sscanf_s
#endif

namespace quotek {

  namespace core {
      
    /**
     * time namespace contains the time related functions of the quotek SDK.
     * Note that for most functions in this namespace user has to provide the current 
     * timestamp instead of just having an internal time(0) call inside. It is because these functions
     * must support backtesting, which implies decorelated real time and backtest time.
     * To circumvent this, all you have to do is to call time functions in your strategies with 
     * this->t as timestamp argument, eg: is_time("10:00", this->t);
     */
    namespace time {


        /** timestr takes an epoch timestamp and returns an "HH:MM:SS" string corresponding.
         *  @param timestamp epoch timestamp to get hour string from.
         *  @return hour string with the form "HH:MM:SS"
         */
         
        std::string timestr(long timestamp);


        /** timeint takes an epoch timestamp and returns a vector of int representing the hours, 
         *  minutes and seconds of timestamp. vect[0] is hours, vect[1] is minutes, vect[2] is seconds
         *  @param timestamp epoch timestamp to get time ints from.
         *  @return an int vector of 3 elements.
         */

        std::vector<int> timeint(long timestamp);

        /**
         * is_time tells if a time string matches with the epoch timestamp provided
         * as an argument.
         * @param time_string string representing time, can be under the form "HH:MM:SS" or "HH:MM"
         * @return true if time_string is current time, false otherwise.
         */

        bool is_time(std::string time_string, 
                     long timestamp);


        /**
         * trade_hours tells if the market is open or not according to open and close hours
         * provided as time strings.
         * @param time_open opening time of asset in market, under the form "HH:MM".
         * @param time_close closing time of asset in market, under the form "HH:MM".
         * @param timestamp epoch
         * @return true if current time is in asset trade hours, false otherwise.
         */

        bool trade_hours(std::string time_open, 
                         std::string time_close, 
                         long timestamp);

        /** week_day returns the long day name according to a provided epoch timestamp.
         * @param timestamp epoch timestamp 
           @return day of week as a string ("monday" or "tuesday", etc..)
         */
        std::string week_day(long timestamp);

        /** month_day provids the number of the day of the month according to provided timestamp. */
        int month_day(long timestamp);

        /** This function implements a portable strptime() function. p_ is for "portable" */
        std::time_t p_strptime(std::string datestr, std::string format);

  
    }
  }
}

#endif
