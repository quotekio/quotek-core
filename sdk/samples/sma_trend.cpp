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

#include <quotek/record.hpp>
#include <quotek/generators.hpp>
#include <quotek/quant.hpp>
#include <iostream>

int main() {

  quotek::data::records current;

  /* we generate a full day of fake Dow-Jones quotations, which represents about 25000 ticks of 1 second */
  quotek::data::records dquotes = quotek::rand::generators::normal(25000, 17000, 100);

  /* Next we'll simulate the running of the quotation day */
  for (int i=0;i< dquotes.size(); i++) {

    current.append(dquotes[i]);

    /* We compute the 2 moving average every 10 ticks */
    if ( i % 10 == 0  ) {

      //we down-sample current for 15-minute ticks
      quotek::data::records current_ds1 = current.down_sample(900,1,"typical");

      //we down-sample current for 50secs ticks.
      quotek::data::records current_ds2 = current.down_sample(50,1,"typical");

      //we compute 20 periods SMA for 15 minutes sample
      std::vector<quotek::data::record> avg1 = quotek::quant::SMA(current_ds1.get_data(),20);

      //we compute 20 periods SMA for 50 seconds sample
      std::vector<quotek::data::record> avg2 = quotek::quant::SMA(current_ds2.get_data(),20);

      if (  percent_delta(avg1) > 0 && percent_delta(avg2) > 0 ) {
        std::cout << "We got a BUY Signal at " << dquotes[i].value << "!" << std::endl;
        // .. We get a potential buy signal, do something..
      }

      else if ( percent_delta(avg1) < 0 && percent_delta(avg2) < 0 ) {
        std::cout << "We got a SELL Signal at " << dquotes[i].value << "!" << std::endl;
        // .. We get a potential sell signal at, do something..
      }
    }
  }
}