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

          const time_t curtime = (const time_t) timestamp;
          struct tm c_time ;
          localtime_r(&curtime,&c_time);

          size_t nb_sep = std::count(time_string.begin(), time_string.end(), ':');

          if (nb_sep == 2) {

            sscanf(time_string.c_str(),"%d:%d:%d",&hour,&minute,&second);

            if (  c_time.tm_hour == hour && 
            	  c_time.tm_min == minute && 
            	  c_time.tm_sec == second ) return true;

            return false;
          }

          else if (nb_sep == 1) {
            sscanf(time_string.c_str(),"%d:%d",&hour,&minute);
            if ( c_time.tm_hour == hour && 
            	 c_time.tm_min == minute ) return true;
            return false;
          }
          else return false;
        
        }
       
        bool trade_hours(std::string time_open, 
        	             std::string time_close, 
        	             long timestamp) {

          //Da fuck ??
          timestamp +=3600;

          struct tm* c_time; 

          std::ostringstream dt_open;
          std::ostringstream dt_close;

          struct tm tm_h1;
          struct tm tm_h2;

          time_t t_h1;
          time_t t_h2;

          c_time = localtime((const time_t*) &timestamp); 

          dt_open << c_time->tm_year+1900 << "-";
          dt_open << std::setfill('0') << std::setw(2) << c_time->tm_mon+1 << "-";
          dt_open << std::setfill('0') << std::setw(2) << c_time->tm_mday;

          dt_close << dt_open.str();

          dt_open << " " << time_open;
          dt_close << " " << time_close;

          strptime(dt_open.str().c_str(),"%Y-%m-%d %H:%M:%S",&tm_h1);
          t_h1 = mktime(&tm_h1);

          strptime(dt_close.str().c_str(),"%Y-%m-%d %H:%M:%S",&tm_h2);
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
          time_t curtime = (time_t) timestamp;
          cweekday = localtime(&curtime)->tm_wday;
          return weekdays[cweekday-1];
        }

        int month_day(long timestamp) {

          int cmday = 1;
          time_t curtime = (time_t) timestamp;
          cmday = localtime(&curtime)->tm_mday;
          return cmday;

        }

    }
  }
}

