/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/csv.hpp>
#include <iostream>

void test_import_records() {

  quotek::datasource::csv c1("file://fixtures/test1.csv",';');
  std::vector<quotek::data::record> recs = c1.import_records(10,2);

  assert( recs.size() == 10 );
  assert( recs[0].value == 4000 );
  
}

void test_import_records_with_timestamp() {

  quotek::datasource::csv c1("file://fixtures/test1.csv",';');
  c1.setTimeColumn(0,"%Y-%m-%d %H:%M:%S");
  std::vector<quotek::data::record> recs = c1.import_records(10,2);

  assert( recs.size() == 10 );
  assert( recs[0].value == 4000 );
  assert( recs[0].timestamp > 0);

}

void test_import_records_with_filters() {

  quotek::datasource::csv c1("file://fixtures/test1.csv",';');
  c1.addFilter("(.*)cac(.*)");
  std::vector<quotek::data::record> recs = c1.import_records(10,2);
 
  assert( recs.size() == 5 );
  assert( recs[0].value == 4000 );

}

/*
void test_import_records_with_offset() {

}

*/

int main() {
  test_import_records();
  test_import_records_with_timestamp();
  test_import_records_with_filters();
}
