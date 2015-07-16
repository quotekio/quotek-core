/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>

namespace quotek {

  namespace core {
    
    /**
     * position class aims to store and manipulate the positions open by 
     * the user's broker. 
     */

    class position {

      public:

      	position();
      	~position();

        /** broker's ticket id */
        std::string ticket_id;

        /** position asset */
        std::string asset_name;

        /** size of the position */
        int size;

        /** Profit & Loss of the position */
        float pnl;

        /** position open price */
      	float open;
      	
      	/** position stop-loss, in BPS */
      	int stop;

        /** position limit, in BPS */
      	int limit;
        
        /** position open date, as epoch timestamp */
        long open_date;
 
        /** date at which the position has been shut */
        long close_date;

        posstats* stats;

      private:

      	/** vstop is a virtual stop-loss */
      	int vstop;

      	/** vlimit */
      	int vlimit;

    };

    /**
     * posstats maintains some statistics about a position over its lifespan.
     */
    class posstats {

      public:
        posstats();
        ~posstats();

        /** pnl_peak indicates the maximum profit over the position's lifespan */
        float pnl_peak;
        
        int vstop_increments;
        int vstop_decrements;



    };


  }
}

#endif
