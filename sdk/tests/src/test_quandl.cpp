/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/quandl.hpp>
#include <iostream>
#include <cmath>

#define EPSILON 0.001

void test_get_db(quotek::datasource::quandl& q) {

  std::vector<quotek::datasource::quandl_db> dblist = q.get_databases();
  assert( dblist.size() > 100 );
  assert ( dblist[0].id != 0  );
  assert ( dblist[0].name != ""  );
  assert ( dblist[0].code != ""  );
  assert ( dblist[0].description != ""  );

}

void test_query(quotek::datasource::quandl& q) {

  std::string res = q.query("WIKI","FB","csv");
  assert (res != "");

  res = q.query("WIKI","FB","csv","","","annual");
  assert(res != "");

}

void test_metadata(quotek::datasource::quandl& q) {

  quotek::datasource::quandl_meta meta1 = q.get_metadata("WIKI","FB");

  assert( meta1.id == 9775687 );
  assert( meta1.name == "Facebook, Inc. (FB) Prices, Dividends, Splits and Trading Volume");
  assert( meta1.code == "FB");
  assert( meta1.description != "");
  assert( meta1.type == "Time Series");
  assert( meta1.frequency == "daily");
  assert( meta1.columns.size() > 0);

}

void test_query_prices(quotek::datasource::quandl& q) {

  quotek::data::records recs = q.query_prices("WIKI","FB");

  assert(recs.size() > 800 );
  assert(recs[0].timestamp == 1337295600 );
  assert( fabs(recs[0].value - 38.23) < EPSILON) ;

}

void test_query_fullprices(quotek::datasource::quandl& q) {

  std::vector<quotek::data::history> hvect = q.query_fullprices("WIKI","FB");

  assert( hvect.size() > 800 );
  assert( hvect[0].timestamp == 1337295600 );
  assert( fabs( hvect[0].open - 42.05) < EPSILON  );
  assert( fabs( hvect[0].high - 45.0) < EPSILON );
  assert( fabs( hvect[0].low - 38.0) < EPSILON);
  assert( fabs( hvect[0].close - 38.23) < EPSILON );

}

void test_get_column(quotek::datasource::quandl& q) {

  std::vector<float> coldata = q.get_column("WIKI","FB",2);
  assert( coldata.size() > 800  );
  assert( fabs( coldata[0] - 45.0) < EPSILON );

}


int main(int argc, char** argv) {

  std::string key = std::string(argv[1]);
  quotek::datasource::quandl q(key);

  test_get_db(q);
  test_query(q);
  test_metadata(q);
  test_query_prices(q);
  test_query_fullprices(q);
  test_get_column(q);

}
