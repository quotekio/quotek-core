/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef QUANDL_HPP
#define QUANDL_HPP

#include <string>
#include "../datasource.hpp"
#include "../http.hpp"
#include "../news.hpp"


namespace quotek {

  namespace datasource {
   
    /** quandl is a class meant to provide a simple quandl API client
     *  Quandl is a very useful service for retrieving lots of financial data coming 
     *  from different sources.
     */
    class quandl: public datasource {

      public:

        /** quandl class constructor. */
      	quandl(std::string api_key);

      	/** quandl class destructor. */
      	~quandl();

        /** get_databases retrieves the full list of databases available on the quandl service. 
      	 *  @return the list of available databases on quandl service.
         */

      	std::vector<std::string> get_databases();

        /** Returns all the datasets contained in a given database. 
         *  @param database database to get datasets for.
         *  @return the list of available datasets
         */

        std::vector<std::string> get_datasets(std::string database);


        /** Performs a query on a prices dataset and returns its result in
         *  a quotek::data::records container. Note that this query only takes the close price.
         *  If you want more data you can make a query_fullprices query.
         *  
         *  @param database database to query prices from.
         *  @param dataset dataset to query prices from.
         *  @param  
         *  @param
         */

        quotek::data::records query_prices(std::string database,
                                           std::string dataset,
                                           const );

        /** Performs a query on a prices dataset and returns its result in a
         *  quotek::data::history (open high low close) container.
         *  
         */

        quotek::data::history query_fullprices(std::string database,
                                               std::string dataset,);

        

        /** query_raw performs a direct query to the quandl API and returns its output
         *  without parsing it, as a string.
         *  @param database database to query from.
         *  @param dataset dataset in database to get data from.
         *  @param 
         *  @param
         */

        std::string query_raw(std::string database,
                              std::string dataset,
                              const );

        std::vector<float> get_column(std::string database,
                                       std::string dataset,
                                       );


        std::string get_metadatas(std::string database,
                                  std::string dataset);


    };

  }
}

#endif
