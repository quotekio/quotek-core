/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <algorithm>
#include "position.hpp"
#include "cvector.hpp"

namespace quotek {

  namespace core {

	/** Portolio is a class meant to store and manage positions in quotek algotrading strategies.
	    It is able to give assessment on risk returns. */ 

	class portfolio {

	  public:

	    /** portfolio constructor 1 
	     *  @param start_capital Starting capital for the portfolio.
	     */
	    portfolio(float start_capital);

        /** portfolio constructor 2
         *  @param start_capital Starting capital for the portfolio.
         *  @positions postions you want to add in the portfolio at its creation.
         */
        portfolio(float start_capital, 
        	      quotek::core::cvector<quotek::core::positions>& positions);

	    /** portfolio destructor */
	    ~portfolio();

        /** update() is a function meant to update the return of each open position 
         *  for asset "asset_name" of the portfolio.
         *  let's say you have two open EURUSD positions in a portfolio p. calling
         *  p.update("EURUSD",1.09) will update the portfolio with the new value of EURUSD (1.09) and update
         *  the positions pnl accordingly.
         *  @param asset_name Name of the asset we want to update pnl for.
         *  @param value current value of the asset.
         */

        void update(std::string asset_name, float value);

        /** total_return gives back the current total return of a portfolio.
         *  @param percent tells if you want return in percentage of portfolio or in absolute value.
         */

        float total_return(bool percent);

        /**
         * compounded returns gives back the time-series of compounded returns for the portfolio's history, 
         * in percentage of capital.
         * @param logarithmic tells if user wants logarithmic or arithmetic comounded returns.
         * @return the compounded returns of the portfolio's history.
         */

        quotek::data::records compounded_returns(bool logarithmic);

        /** protit_factor computes the profit factor for the whole portfolio's history. 
         *  @return the profit factor of the portfolio's history.
         */
        float profit_factor();

        /** drawdown computes the max drawdown of the portfolio's history. 
         *  @return the drawdown the of the portfolio's history.
         */
        float drawdown();
        
	    /** positions stores the list of currently open positions. */
	  	quotek::core::cvector<quotek::core::positions> positions;

	    /** history stores the previously open positions of the portfolio, 
	        but which are closed now. */
	  	quotek::core::cvector<quotek::core::positions> history;

	};

  }

}
