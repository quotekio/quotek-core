/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/twitter.hpp>
#include <iostream>
#include "time.h"


void test_fetch(quotek::datasource::twitter& tw) {

  std::vector<quotek::data::news> lnews = tw.fetch("WSJmarkets", 5);

  assert(lnews.size() == 5) ;
  assert(lnews[0].content != "");
  assert(lnews[0].date + 86400 > time(NULL) );
  assert(lnews[0].source == "twitter:WSJmarkets");

}

void test_search(quotek::datasource::twitter& tw) {

  std::vector<quotek::data::news> lnews = tw.search("DAX", 5);
  assert(lnews.size() == 5) ;
  assert(lnews[0].content != "");
  
  std::cout << lnews[0].source << std::endl;
  std::cout << lnews[0].content << std::endl;

}

int main(int argc, char** argv) {

  if ( argc < 5 ) exit(1);
  
  quotek::datasource::twitter tw(argv[1],
                                 argv[2],
                                 argv[3],
                                 argv[4]);

  test_fetch(tw);
  test_search(tw);
}
