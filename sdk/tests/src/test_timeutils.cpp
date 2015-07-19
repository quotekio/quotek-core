/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/
#include <quotek/broker.hpp>
#include <quotek/timeutils.hpp>

void test_is_time(std::string tstr, long t) {
  assert( quotek::core::time::is_time(tstr,t) );
}

void test_trade_hours(std::string tstr1, std::string tstr2, long t) {
  assert( quotek::core::time::trade_hours(tstr1,tstr2,t) == true ); 
}

void test_week_day(long t) {
  assert( quotek::core::time::week_day(t) == "monday" );
}

void test_month_day(long t) {
  assert ( quotek::core::time::month_day(t) == 20 );
}

int main() {

  std::string tstr = "10:49";
  std::string tstr1 = "09:00:00";
  std::string tstr2 = "17:30:00";

  long t = 1437382175;

  test_is_time(tstr,t);
  test_trade_hours(tstr1,tstr2,t);
  test_week_day(t);
  test_month_day(t);

}
