/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

//https://openbook.etoro.com/api/Markets/Category?category=currencies


#ifndef ETORO_HPP
#define ETORO_HPP

#include "../datasource.hpp"
#include "../utils.hpp"
#include "../http.hpp"
#include "../position.hpp"
#include "../timeutils.hpp"

#include <vector>
#include <iostream>
#include <sstream>

namespace quotek {

  namespace datasource {

        class eto_user {
          public:
          	eto_user() {}
          	~eto_user() {}
        
          	/** user name. */
            std::string username;
        
            /** Maximum drawdown. */
            float max_drawdown;
        
            /** Number of copiers follwing the user. */
            int copiers;
        
            /** Amount of gain/losses in percents */
            float gain;

            /** Number of weeks the user was in profit. */
            float profitable_weeks;

            /** Rank of the user */
            int rank;

        };

        /** etoro is a class meant to make easy queries to the eToro OpenBook API. */

		class etoro: public datasource {

		  public:

            /** Class Constructor. */
		  	etoro();

		  	/** Class Destructor. */
		  	~etoro();

            /** Gets a ranking of the eToro users given specific parameters.
             *  @param period Time period in days, for which you want to get the ranking.
             *  @param risk_level Tells for which kind of risk profile you want the ranking. 
             *  risk_level can be "Low", "Medium" or "High".
             *  @param sort_type Tells how you want to sort the users list sorted. Can be "Copiers" or "Gain".
             *  @return a list of eto_users.
             */

		  	std::vector<quotek::datasource::eto_user> ranking(int period, 
		  		                       std::string risk_level,
		  		                       std::string sort_type);

            /** A little like ranking(), top returns the list of the 5 first eToro users that perfomed the most by copiers
             *  on 3 , 6 and 12 months period of time.
             *  @return a 3 elements vector of ranked users vectors
             */

            std::vector< std::vector<quotek::datasource::eto_user> > top();
            

            /** Gets the eToro portolio of an user, given his/her username and extra parameters.
             *  @param username User Name of the user you want to get the portfolio for.
             *  @param pos_type Type of positions you want to retrieve. pos_type can be "Real" or "Demo".
             *  @param pos_time Tells if you to get current or historical positions. pos_time can be "Current" or "History".
             *  @return The user's portfolio, under the form of vector of quotek position objects.
             */

		  	std::vector<quotek::core::position> positions(std::string username,
		  		                                          std::string pos_type,
		  		                                          std::string pos_time);

            /** Symbols fetch the full list of eToro symbols for a given market category.
             *  @param category Market Category we want to get symbols for. Can be either "currencies",
             *  "commodities", "indices" and "stocks".
             *  @return the list of symbols for the given market.
             */

            std::vector<std::string> symbols(std::string category);

            /** insight is a function returning the long/short ratio of the eToro users positions 
             *   for a given market. If open positions on GOLD are 70% long and 30% short, 
             *   then the result of insight("Gold","commodities") will be (70,30).
             *   
             *   @param symbol symbol of the market we want insight for.
             *   @param category Market Category the symbol is in. Can be either "currencies",
             *   "commodities", "indices" and "stocks". If you want insight for symbol "Gold" 
             *   then category will be "commodities", for "EUR/USD" it will be "currentcies", etc..
             *
             *   @return a 2-elements vector of float number containing both the long ratio and the short ratio.
             */       

            std::vector<float> insight(std::string symbol, std::string category);

		  private:

		  	static std::string base_url;

		};

  }
}

#endif