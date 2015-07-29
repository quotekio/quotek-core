/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/yahoofinance.hpp>

void test_history() {

  quotek::datasource::yahoofinance y1;
  std::vector<quotek::data::history> hist = y1.history(quotek::datasource::yahoofinance::CAC40,
                                                       "2015-07-19",
                                                        "2015-07-24");

  assert( hist.size() == 5 );
  assert( hist[0].open > 4000  && hist[0].open < 6000);

}

void test_close_history() {

  quotek::datasource::yahoofinance y1;
  quotek::data::records hist = y1.close_history(quotek::datasource::yahoofinance::CAC40,
                                                       "2015-07-19",
                                                        "2015-07-24");
  
  assert( hist.size() == 5 );
  assert( hist[0].value > 4000  && hist[0].value < 6000);

}


int main() {

  test_history();
  test_close_history();

}
