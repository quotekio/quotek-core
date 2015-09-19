/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "timeutils.hpp"

namespace quotek {

  namespace core {
      
    namespace time {
       

        bool is_time(std::string time_string, long timestamp) {

          int hour, minute, second;

          size_t nb_sep = std::count(time_string.begin(), time_string.end(), ':');

          const std::time_t ttstamp = (const std::time_t) timestamp;
          std::tm* c_time =  std::localtime(&ttstamp);

          if (nb_sep == 2) {

            sscanf(time_string.c_str(),"%d:%d:%d",&hour,&minute,&second);

            if (  c_time->tm_hour == hour && 
                c_time->tm_min == minute && 
                c_time->tm_sec == second ) return true;

            return false;
          }

          else if (nb_sep == 1) {
            sscanf(time_string.c_str(),"%d:%d",&hour,&minute);
            if ( c_time->tm_hour == hour && 
               c_time->tm_min == minute ) return true;
            return false;
          }
          else return false;

        }

        bool trade_hours(std::string time_open, 
        	             std::string time_close, 
        	             long timestamp) {

          
          std::time_t ttstamp = (const std::time_t) timestamp;
          std::tm* c_time; 

          std::ostringstream dt_open;
          std::ostringstream dt_close;

          std::tm tm_h1;
          std::tm tm_h2;

          std::time_t t_h1;
          std::time_t t_h2;

          c_time = localtime(&ttstamp); 

          dt_open << c_time->tm_year+1900 << "-";
          dt_open << std::setfill('0') << std::setw(2) << c_time->tm_mon+1 << "-";
          dt_open << std::setfill('0') << std::setw(2) << c_time->tm_mday;

          dt_close << dt_open.str();

          dt_open << " " << time_open;
          dt_close << " " << time_close;

          sscanf(dt_open.str().c_str(),"%d-%d-%d %d:%d:%d",
          &tm_h1.tm_year,
          &tm_h1.tm_mon,
          &tm_h1.tm_mday,
          &tm_h1.tm_hour,
          &tm_h1.tm_min,
          &tm_h1.tm_sec  
          );

          tm_h1.tm_year -= 1900;
          tm_h1.tm_mon -= 1;

          t_h1 = std::mktime(&tm_h1);

          sscanf(dt_close.str().c_str(),"%d-%d-%d %d:%d:%d",
          &tm_h2.tm_year,
          &tm_h2.tm_mon,
          &tm_h2.tm_mday,
          &tm_h2.tm_hour,
          &tm_h2.tm_min,
          &tm_h2.tm_sec  
          );

          tm_h2.tm_year -= 1900;
          tm_h2.tm_mon -= 1;

          t_h2 = mktime(&tm_h2);

          if (timestamp >= t_h1 && timestamp <= t_h2 ) return true;
          else return false;

        }



        std::string week_day(long timestamp) {

          std::vector<std::string> weekdays = { "monday", 
                                                "tuesday",
                                                "weddnesday",
                                                "thursday",
                                                "friday",
                                                "saturday",
                                                "sunday" };
          int cweekday = 1;
          const std::time_t curtime = (const std::time_t) timestamp;
          cweekday = std::localtime(&curtime)->tm_wday;
          return weekdays[cweekday-1];
        }

        int month_day(long timestamp) {

          int cmday = 1;
          const std::time_t curtime = (const std::time_t) timestamp;
          cmday = std::localtime(&curtime)->tm_mday;
          return cmday;

        }

        std::time_t p_strptime(std::string datestr, std::string format) {

          std::tm tm;

          #ifndef _WIN64
            strptime(datestr.c_str(),format.c_str(),&tm);
          #else
            std::stringstream ss(datestr);
            ss >> std::get_time(&tm, format.c_str());
          #endif

          return mktime(&tm);
          
        }

    }
  }
}

