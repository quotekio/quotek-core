/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/etoro.hpp>
#include <iostream>

void test_ranking() {

  quotek::datasource::etoro et1;
  std::vector<quotek::datasource::eto_user> elist = et1.ranking(30,"Medium","Gain");

  assert( elist.size() > 0 );
  assert( elist[0].username != "" );
  assert( elist[0].rank == 1 );
  assert( elist[0].max_drawdown > 0 );
  assert( elist[0].profitable_weeks > 0 );
  
}

void test_top() {

  quotek::datasource::etoro et1;
  std::vector< std::vector<quotek::datasource::eto_user> > tlist = et1.top();

  assert(tlist.size() == 3);
  assert(tlist[0].size() == 5);
  assert(tlist[1].size() == 5);
  assert(tlist[2].size() == 5);

  assert(tlist[0][0].username != "");
  assert(tlist[0][0].rank == 1);
  assert(tlist[0][0].copiers > 0);
  assert(tlist[0][0].gain > 0);


}


void test_insight() {

  quotek::datasource::etoro et1;
  std::vector<float> rt = et1.insight("Gold","commodities");
  
  assert( rt.size() == 2 );
  assert( rt[0] > 0 );
  assert( rt[1] > 0 );

}


void test_symbols() {

  quotek::datasource::etoro et1;
  std::vector<std::string> slist = et1.symbols("indices");

  assert(slist.size() > 0);
  assert(slist[0] != "");
 
}

void test_positions() {

  quotek::datasource::etoro et1;
  std::vector<quotek::datasource::eto_user> elist = et1.ranking(30,"Medium","Gain");

  std::vector<quotek::core::position> plist = et1.positions(elist[1].username,"real","current");

  assert(plist.size() > 0);
  assert(plist[0].pnl != 0 );
  assert(plist[0].asset_name != "");
  assert(plist[0].open_date > 0);
  assert(plist[0].close_date == 0);

  plist = et1.positions(elist[0].username,"real","history");

  /*
  assert(plist.size() > 0);
  assert(plist[0].pnl != 0 );
  assert(plist[0].asset_name != "");
  assert(plist[0].close_date > 0);
  assert(plist[0].open_date == 0);
  */

}


int main() {

  test_ranking();
  test_top();
  test_insight();
  test_symbols();
  test_positions();

  exit(0);

}
