/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef HISTORY_HPP
#define HISTORY_HPP

namespace quotek {

  namespace data {

    /**
     * history is a container which aims to store historical data of an asset fetched from 
     * a remote or local datasource. While quotek::data::record is meant to store raw
     * asset values provided by broker in real-time, history is more about storing historical data having 
     * a longer tick, usually the kind of data available from quotes information providers.
     */
    class history {

      public:
      
       /** history constructor */
       history();       

       /** history constructor 2 */
       history(long timestamp, 
              float open, 
              float close,
              float high,
              float low);

       /** history destructor */
       ~history ();

       /** stores the timestamp on which the quotation begun (usually beginning of the day), in epoch format.*/
       long timestamp;

       /** stores the opening price of the asset for the period. */
       float open;
 
       /** stores the (non-adjusted) closing price of the asset for the period. */
       float close;

       /** stores the highest price of the asset for the period. */
       float high;

       /** stores the lowest price of the asset for the period. */
       float low;
    };

  }

}
#endif