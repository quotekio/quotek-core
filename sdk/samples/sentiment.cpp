/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io

Disclaimer
----------
This source code is provided for educating purposes ONLY, and MUST NOT be used as 
a real investment strategy. Therefore, Quotek SAS declines all responsability for
potential loss or missed investment opportunities resulting of the misuse of these 
algorithms in real situations.

*/

#include <iostream>
#include <quotek/alchemy.hpp>
#include <quotek/datasources/rss.hpp>

int main() {

  int pos_sent = 0;
  int neg_sent = 0;

  /* we declare a new AlchemyAPI client object. 
   * Here we must provide the Alchemy API-key and API endpoint. 
   */

  quotek::ml::alchemy a1("DKJSKSJD-OIEEOIDJS-LQKSLQSKQLS","http://access.alchemyapi.com");

  /* we declare a new RSS data source, with the WSJ rss feed url as argument */

  quotek::datasource::rss s1("http://www.wsj.com/xml/rss/3_7031.xml");

  /* we fetch the last WSJ market news in the feed */
  std::vector<quotek::data::news> wsj_nlist = s1.fetch(5);

  for (int i=0;i< wsj_nlist.size();i++) {

    /* we send each news to the Alchemy API service for sentiment analysis. */
    quotek::ml::sentiment sent = a1.sentiment( wsj_nlist[i].content,"","text");

    if (sent.positive) pos_sent++;
    else neg_sent++;

  }

  if ( pos_sent > neg_sent ) {
   std::cout << "sentiment on US markets is positive overall" << std::endl;
   /* .. take decision here */
  }

  else {
    std::cout << "sentiment on US markets is negative overall" << std::endl;
    /* .. take another decision here */
  }

}