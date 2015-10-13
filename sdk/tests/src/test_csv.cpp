/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/quant.hpp>
#include <quotek/datasources/csv.hpp>
#include <iostream>

#define EPSILON 0.01

void test_import_records() {

  quotek::datasource::csv c1("file://fixtures/test1.csv",';');
  quotek::data::records recs = c1.import_records(10,2);

  assert( recs.size() == 10 );
  assert( recs[0].value == 4000 );
  
}

void test_import_records_with_timestamp() {

  quotek::datasource::csv c1("file://fixtures/test1.csv",';');
  c1.setTimeColumn(0,"%Y-%m-%d %H:%M:%S");
  quotek::data::records recs = c1.import_records(10,2);

  assert( recs.size() == 10 );
  assert( recs[0].value == 4000 );
  assert( recs[0].timestamp > 0);

}

void test_import_records_with_filters() {

  quotek::datasource::csv c1("file://fixtures/test1.csv",';');
  c1.addFilter("(.*)cac(.*)");
  quotek::data::records recs = c1.import_records(10,2);
 
  assert( recs.size() == 5 );
  assert( recs[0].value == 4000 );

}


void test_import_history() {

  quotek::datasource::csv c1("file://fixtures/test2.csv",';');
  c1.setOffset(1);
  std::vector<quotek::data::history> cac_hist = c1.import_history(10,0,1,2,3);

  assert(cac_hist.size() == 10);
  assert( fabs(cac_hist[0].open - 4001.2) < EPSILON  );

}


int main() {
  test_import_records();
  test_import_records_with_timestamp();
  test_import_records_with_filters();
  test_import_history();
}
