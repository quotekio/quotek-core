/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef QUANDL_HPP
#define QUANDL_HPP

#include <string>
#include "csv.hpp"
#include "../datasource.hpp"
#include "../history.hpp"
#include "../http.hpp"
#include "../record.hpp"

namespace quotek {

  namespace datasource {
   

    /** quandl_meta is a class meant to store quandl dataset metadata. */

    class quandl_meta {

      public:

        quandl_meta() {}
        ~quandl_meta() {}

        /** id of the dataset. */
        int id;

        /** name of the dataset. */
        std::string name;

        /** code of the dataset . */
        std::string code;

        /** type of the dataset (time series, etc..) */
        std::string type;
        /** description of the dataset */
        std::string description;

        /** columns inside the dataset */
        std::vector<std::string> columns;

        /** update frequency of the dataset */
        std::string frequency;

    };

    /** quandl_db is a class created to store information relative to databases, fetched via get_databases() */
    
    class quandl_db {

      public:

        quandl_db() {}
        ~quandl_db() {}
    
        /** id of the database. */
        int id;

        /** long name of the database. */
        std::string name;

        /** short code of the database. */
        std::string code;

        /** description of the database. */

        std::string description;

        /** number of datasets stored in the database. */
        int datasets;

    };

    /** quandl is a class meant to provide a simple quandl API client.
     *  Quandl is a very useful service for retrieving lots of financial data coming 
     *  from various sources.
     */

    class quandl: public datasource {

      public:

        /** quandl class constructor. 
         *  @param api_key Account API Key provided by quandl.
         *  
         *  Note: you can add an empty api_key "" but then are limited to 50 queries/day.
         */
         
      	quandl(std::string api_key);

      	/** quandl class destructor. */
      	~quandl();

        /** get_databases retrieves the full list of databases available on the quandl service. 
         *  @param pgnum Starting page number you want to get data from. Mostly for recursion, you shouldn't take care of this.
      	 *  @return the list of available databases on quandl service , as quotek::datasource::quandl_db objects.
         */

      	std::vector<quotek::datasource::quandl_db> get_databases(const int pgnum = 0);
        
         /** get_metadatas allows to retrieve the metadata associated with a given dataset.
         *  @database database to get metadata from.
         *  @dataset dataset to get metadata from.
         *  @return a quotek::datasource::quandl_meta object containing the metadata info.
         */

        quotek::datasource::quandl_meta get_metadata(std::string database,
                                                std::string dataset);

        /** query() performs a direct query to the quandl API and returns its output
         *  without parsing it, as a string.
         *  @param database database to query from.
         *  @param dataset dataset in database to get data from.
         *  @param format return format needed. Can be "csv", "json" and "xml".
         *  @return the resulting dataset returned by quandl API, as a string.
         *  @param start_date YYYY-mm-dd string representation of the first day of data you want in the set.
         *  @param end_date YYYY-mm-dd string representation of the last day of data you want in the set.
         *  @param collapse aggregation of your data. Can be "daily", "weekly", "monthly", "quarterly", "annual".
         *  @param transform data arithmetic, can be "diff","rdiff","cumul","normalize". 
         *  @param sort_order order in which data will be returned. Can be "asc" or "desc".
         *  @param nb_rows number of rows to return in the dataset.
         *  @param column_num If you want to return only a specific column of the dataset, use this parameter.
         *  @param exclude_colnames Put this at true if you want to remove the columns names at the begining of the dataset.
         */

        std::string query(std::string database,
                              std::string dataset,
                              std::string format,
                              const std::string start_date = "",
                              const std::string end_date = "",
                              const std::string collapse = "",
                              const std::string transform = "",
                              const std::string sort_order = "",
                              const int nb_rows = -1,
                              const int column_num = -1,
                              const bool exclude_colnames = false
                              );


        /** Performs a query on a prices dataset and returns its result in
         *  a quotek::data::records container. Note that this query only takes the close price.
         *  If you want more data you can make a query_fullprices query.
         *  
         *  @param database database to query prices from.
         *  @param dataset dataset to query prices from.
         *  @return a quotek::data::records container, containing all the datas of the set.
         *  @param start_date YYYY-mm-dd string representation of the first day of cotation you want in the set.
         *  @param send_date YYYY-mm-dd string representation of the last day of cotation you want in the set.
         *  @param collapse aggregation level of your data. Can be "daily", "weekly", "monthly", "quarterly", "annual".
         *  
         */

        quotek::data::records query_prices(std::string database,
                                           std::string dataset,
                                           const std::string start_date = "",
                                           const std::string end_date= "",
                                           const std::string collapse = "" );

        /** Performs a query on a prices dataset and returns its result in a
         *  vector of quotek::data::history objects (open high low close).
         *  
         *  @param database database to query prices from.
         *  @param dataset dataset to query prices from.
         *  @return a vector of quotek::data::history objects, containing all the datas of the set.
         *  @param start_date YYYY-mm-dd string representation of the first day of cotation you want in the set.
         *  @param send_date YYYY-mm-dd string representation of the last day of cotation you want in the set.
         *  @param collapse aggregation level of your data. Can be "daily", "weekly", "monthly", "quarterly", "annual".
         *  
         */
         
        std::vector<quotek::data::history> query_fullprices(std::string database,
                                               std::string dataset,
                                               const std::string start_date = "",
                                               const std::string end_date = "" ,
                                               const std::string collapse = "");

        
        /** Returns a specific data column of float values.
         *  @param database database to get data from.
         *  @param dataset dataset to get data from.
         *  @param column_num column number to fetch.
         *  @return a list of float values.
         *  @param start_date YYYY-mm-dd string representation of the first day of data you want in the set.
         *  @param send_date YYYY-mm-dd string representation of the last day of data you want in the set.
         *  @param collapse aggregation level of your data. Can be "daily", "weekly", "monthly", "quarterly", "annual".
         */

        std::vector<float> get_column(std::string database,
                                       std::string dataset,
                                       int column_num,
                                       const std::string start_date = "",
                                       const std::string end_date = "",
                                       const std::string collapse = "");
                           
      private:

        std::string api_key;
        static std::string root_url ;

    };

  }
}

#endif
