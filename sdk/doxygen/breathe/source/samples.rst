SDK Examples
============

Disclaimer
----------
*The strategies provided here are for educating purposes ONLY, and MUST NOT be used as 
real investment strategies. Therefore, Quotek SAS declines all responsability for
potential loss or missed investment opportunities resulting of the misuse of these 
algorithms in real situations.*

Stock market sentiment analysis with Alchemy API
------------------------------------------------

In this example we will use the Alchemy API service to evaluate the overall sentiment of the US equity market using the content provided by the Wall-Street Journal RSS feed.

.. code-block:: c++

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

      if (sent.sentiment == "positive") pos_sent++;
      else if (sent.sentiment == "negative") neg_sent++;

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


Moving Averages evolution on multiple time scales
-------------------------------------------------

For this example we will try to deduce buy and sell signals for an asset by using 2 SMA20 moving averages on 50seconds and 15minutes time scales.

Predicates
^^^^^^^^^^

  * If the 2 moving averages go up at the same time, then we might have a buy signal. 
  * On the other side if they both go down at the same time we may have a sell signal.
  * If the 2 moving average don't go the same way, then we have no signal at all.
  
Also in order see some things, we will simulate a full day of dow-jones quotation by using a random, normally distributed sample (17000, sigma of 100).

.. highlight ::c++

.. code-block:: c++

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
