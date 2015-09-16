/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef RECORD_HPP
#define RECORD_HPP

#include <time.h>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>

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

       /** 
        * min is a replicate of quotek::quant::min in record namespace.
        * @param recs dataset to find minimum for.
        * @return lowest value inside dataset.
        */
       static float min(std::vector<record>& recs);

       /** 
        * max is a replicate of quotek::quant::max in record namespace.
        * @param recs dataset to find maximum for.
        * @return highest value inside dataset.
        */
       static float max(std::vector<record>& recs);

       /** stores the epoch timestamp at which the asset was worth value. */
       long timestamp;

       /** the value of the asset at time timestamp. */
       float value;
       /** the spread offered by the broker for the asset at time timestamp. */ 
       float spread;

    };

    /** records is a class that is is a quotek::data::record container */

    class records {

      public:
        /** Class Constructor */
        records();

        /** Class constructor 2.
         *  Takes a  vector of (timestamp,value,spread) tuples to 
         *  construct a records object.
         *  @param data vector of (long,float,float) tuples.
         */
        records(std::vector<std::tuple<long,float,float>>& data);

        /** Class constructor 3.
         * Takes a vector of float values to construct a records object.
         * With this constructor, spread is systematically left to 0 and timestamp ranges (1..data.size())
         * @param data vector of float.
         */
         records(std::vector<float>& data);

         /** Class constructor 4.
          * Takes a vector of quotek::data::record values to construct a records object.
          * @param data vector of quotek::data::record.
          */
        records(std::vector<quotek::data::record>& data);
         


        /** Class Destructor */
        ~records();

        /** [] Operator */
        quotek::data::record& operator [] (const int&  i) {
          return this->data[i];
        }

        /** Downsample reduces the amount of points in the dataset by 
         *  agrregating data in "period" intervals.
         *  @param period size of the aggregate, in seconds.
         *  @param tick tick exprimed in seconds of records dataset.
         *  @method down sampling method. Method can be either "close", "HL2", "typical", "OHLC4".
         *  @return the new, down sampled dataset.
         *
         *  How down sampling methods work:
         *  - close: Simply takes the last price over aggregated time period.
         *  - HL2: Takes high and low price over aggregated time period and makes average.
         *  - typical: Takes high, low and close prices over time period and makes average.
         *  - OHLC4: Takes open, high, low and close prices over time period and makes average.
         
         * Note about down sampling: spread values are not kept in resulting dataset
         * it would not make any sense to do so. therefore, they're all 0 in resulting dataset.
         */

        quotek::data::records down_sample(int period, 
                                         float tick, 
                                         std::string method);


        /** Retrieves a vector of quotek::data::record. */
        std::vector<quotek::data::record>& get_data();

        /** Retrieves a vector that contains all the timestamps of the dataset. */
        std::vector<long>& get_timestamps();

        /** Retrieves a vector that contains all the values of the dataset. */
        std::vector<float>& get_values();

        /** Retrieves a vector that contains all the spreads of a dataset. */
        std::vector<float>& get_spreads();

        /** last returns last vector element. */
        quotek::data::record& last();

        /** returns size of container. */
        size_t size();

        /** Adds new entry to records container (no spread) */
        void append(long timestamp, float value);

        /** Adds new entry to records container (with spread) */
        void append(long timestamp, float value, float spread);

        /** Adds new entry to records container (record form) */
        void append(quotek::data::record& r);

      private:
        std::vector<quotek::data::record> data;
        std::vector<long> timestamps;
        std::vector<float> values;
        std::vector<float> spreads;
    };
  
  }

}
#endif