/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef RECORD_HPP
#define RECORD_HPP

#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>

#include "eigen3/Eigen/Dense"


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
        * str takes the data inside the record object and
        * transforms it to a json-formated string.
        * @param add_timestamps if true, record timestamp will be added to the string.
        * @return the content of the record, formatted as a JSON string.
        */         
       std::string str(const bool add_timestamps);

       /** stores the epoch timestamp at which the asset was worth value. */
       long timestamp;

       /** the value of the asset at time timestamp. */
       float value;
       /** the spread offered by the broker for the asset at time timestamp. */ 
       float spread;

       /** Translates record to Eigen::VectorXd (dvector) type.
        *  @param add_timestamps tells if timestamps must be added to vector or not.
        *  @return the record translated as Eigen::VectorXd (dvector)
        */
       Eigen::VectorXd to_vector(const bool add_timestamps);

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

        /**
         * search performs a binary search inside a vector of records given a timestamp.
         * @param recs the record dataset to search in
         * @param search_timestamp the timestamp to search for inside the dataset.
         * @return index of the found record element inside the dataset.
         */
        int search(long search_timestamp);

        /**
         * sample is a method to extract a subvector of records from a vector of records, given starting and ending timestamps.
         * Note: timestamps can be negative, in which case the real timestamps are time() + time_inf, time() + time_sup.
         * @param recs the record dataset to extract sample from.
         * @param time_inf epoch timestamp for first element of sample.
         * @param time_sup epoch timestamp for the last element of sample.
         * @return resampled dataset containing only the data in time interval [time_inf, time_sup]
         */
         quotek::data::records sample(long time_inf, long time_sup);
        
        /**
         * 
         * to create a new subvector.
         * @param recs record dataset to extract data from.
         * @param start_offset the element index from which to start extract
         * @param size the size of the extract.
         * @return extraced dataset containing data in interval [start_offsret, start_offset + size]
         */
         quotek::data::records extract(int start_offset, int size );

         /**
          * Alternative extract function, for extraction of the last elements of a quotek::data::records container.
          * @param n number of last elements to add in the returned container.
          * @return a quotek::data::records container containing the n last elements of the object.
          */
         quotek::data::records extract(int n);


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
        std::vector<long> export_timestamps();

        /** Retrieves a vector that contains all the values of the dataset. */
        std::vector<float> export_values();

        /** Retrieves a vector that contains all the spreads of a dataset. */
        std::vector<float> export_spreads();

        /** last returns last vector element. */
        quotek::data::record& last();

        /** returns size of container. */
        size_t size();

        /** Adds new entry to records container (record form) */
        void append(quotek::data::record& r);

        /** Adds new entry to records container (value only) */
        void append(float value);

        /** Adds new entry to records container (no spread) */
        void append(long timestamp, float value);

        /** Adds new entry to records container (with spread) */
        void append(long timestamp, float value, float spread);

        /** concatenates 2 records datasets to 1 */
        void append(quotek::data::records& recs);

         /**
         * str takes the data inside the records container and
         * transforms it to a json-formated string.
         * @param add_timestamps if true, record timestamp will be added to the string.
         * @return the content of the records container, formatted as a JSON string.
         */

         std::string str(const bool add_timestamps);

        /**
         * finds record which has the smallest value.
         */
        quotek::data::record min();

        /**
         * finds record which has the highest value.
         */
        quotek::data::record max();

        /** Similar to export_values, but instead of returning an std::vector<float>, it
         *  returns an Eigen::VectorXd (dvector) data structure.
         *  @return all the values alligned inside an Eigen::VectorXd (dvector)
         */
        Eigen::VectorXd to_vector();

        /** Converts the quotek::data::records structure to an Eigen::MatrixXd (dataset) structure.
         *  @param add_timestamps tells if timestamps must be added to vector or not.
         *  @return an Eigen::MatrixXd (dataset) representation of the data.
         */
        Eigen::MatrixXd to_matrix(const bool add_timestamps);

      private:
        std::vector<quotek::data::record> data;
    };
  

    /** Adds possibility to add records directly in stringstream */
    std::ostream& operator<<(std::ostream& stream, quotek::data::records& r);
    
    /** Adds possibility to add record directly in stringstream */
    std::ostream& operator<<(std::ostream& stream, quotek::data::record& r);



  }

}
#endif