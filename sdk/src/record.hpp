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
        * @param recs the record dataset to search in
        * @param search_timestamp the timestamp to search for inside the dataset.
        * @return index of the found record element inside the dataset.
        */
       static int search(std::vector<record>& recs,
                         long search_timestamp);

       /**
        * sample is a method to extract a subvector of records from a vector of records, given starting and ending timestamps.
        * Note: timestamps can be negative, in which case the real timestamps are time() + time_inf, time() + time_sup.
        * @param recs the record dataset to extract sample from.
        * @param time_inf epoch timestamp for first element of sample.
        * @param time_sup epoch timestamp for the last element of sample.
        * @return resampled dataset containing only the data in time interval [time_inf, time_sup]
        */
       static std::vector<record> sample(std::vector<record>& recs,
                     long time_inf,
                     long time_sup);
 
       /**
        * extract is a conviency subvector extract. it takes begin() + start_offset, begin + start_offset + size iterators
        * to create a new subvector.
        * @param recs record dataset to extract data from.
        * @param start_offset the element index from which to start extract
        * @param size the size of the extract.
        * @return extraced dataset containing data in interval [start_offsret, start_offset + size]
        */
       static std::vector<record> extract(std::vector<record>& recs, int start_offset, int size );

       /**
        * tima_as_value allows to place timestamps contained in records as values.
        */
       static std::vector<record> time_as_value(std::vector<record>& recs);
      
       /**
        * values_export is a static method that takes all the values contained in the record dataset to reformat 
        * it as a simple floats vector.
        * @param recs dataset to export.
        * @return vector of float values.
        */
       static std::vector<float> values_export(std::vector<record>& recs);

       /**
        * timestamps_export is a static method that takes all the the timestamps contained in the record dataset
        * to reformat it as a simple long vector.
        * @param recs dataset to export timestamps from.
        * @return vector of long timestamps.
        */
       static std::vector<long> timestamps_export(std::vector<record>& recs);


       /**
        * values_import is a static method which creates a records vector from a vector of float values, 
        * it makes the exact opposite as export()
        * @param vector of floats to import.
        * @return vector of records (the usual quotek dataset format)
        */
       static std::vector<record> values_import(std::vector<float>& recs);

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