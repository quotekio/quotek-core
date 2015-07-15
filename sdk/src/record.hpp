/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef RECORD_HPP
#define RECORD_HPP

#include <time.h>
#include <vector>

namespace quotek {

  namespace data {

    /**
     * Record is a container class specially crafted to store asset values data.
     */

    class record {

      public:
         
       /** record constructor */
       record();       

       /** record constructor 2 */
       record(long timestamp, 
              float value, 
              float spread);
 
       /** record destructor */
       ~record();

       /**
        * search performs a binary search inside a vector of records given a timestamp.
        */
       static int search(std::vector<record>& recs,
                         long search_timestamp);

       /**
        * sample is a method to extract a subvector of records from a vector of records.
        */
       static std::vector<record> sample(std::vector<record>& recs,
                     long time_inf,
                     long time_sup);

       /** stores the epoch timestamp at which the asset was worth value. */
       long timestamp;
       /** the value of the asset at time timestamp. */
       float value;

       /** the spread offered by the broker for the asset at time timestamp. */ 
       float spread;

    };
}

}
#endif