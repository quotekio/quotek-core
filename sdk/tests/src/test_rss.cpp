/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/rss.hpp>
#include <iostream>

void test_fetch() {

  quotek::datasource::rss r1("http://syndication.lesechos.fr/rss/rss_finance-marches.xml");
  std::vector<quotek::data::news> nvect = r1.fetch(2);
  
  assert(nvect.size() == 2);
  assert(nvect[0].date > 0);
  assert(nvect[0].title != "" );
  assert(nvect[0].content != "" );
  
}


int main() {

  test_fetch();

  exit(0);

}
