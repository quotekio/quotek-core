/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef YAHOOFINANCE_HPP
#define YAHOOFINANCE_HPP

#include <string>
#include "../datasource.hpp"
#include "../history.hpp"
#include "../http.hpp"
#include "../record.hpp"

namespace quotek {

  namespace datasource {
   
    /** yahoofinance is a class which allows to query the Yahoo! Finance API service to 
     *  fetch historical data on stocks and indices.
     */
    class yahoofinance: public datasource {

      public:

        /** yahoofinance class constructor */
      	yahoofinance();

      	/** yahoofinance class destructor */
      	~yahoofinance();

        /** returns an asset history under the form of a quotek history vector (open,close,high,low).
         *  @param symbol yahoo! symbol for the asset.
      	 *  @param from string-formated date with the format "YYYY-MM-DD".
      	 *  @param to string-formated date with the format "YYYY-MM-DD".
      	 *  @return a vector of quotek history records.
         */
      	std::vector<quotek::data::history> history(std::string symbol,
      		                                       std::string from, 
      		                                       std::string to);

      	/** returns the closing history of an asset defined by symbol.
      	 *  Compared to history, it is a very convenient function to perform 
      	 *  moving average, VAR, etc.. computations.
      	 *  @param symbol yahoo! symbol for the asset.
      	 *  @param from string-formated date with the format "YYYY-MM-DD".
      	 *  @param to string-formated date with the format "YYYY-MM-DD".
      	 *  @return a quotek records structure containing closing prices as value.
      	 */
      	quotek::data::records close_history(std::string symbol,
      		                                std::string from, 
      		                                std::string to);

        static std::string CAC40;
        static std::string DAX;
        static std::string DOW;
        static std::string SP500;
        static std::string NASDAC;
        static std::string NIKKEI;

    };


  }
}

#endif
