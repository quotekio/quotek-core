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
     * posstats maintains some statistics about a position over its lifespan.
     */
    class posstats {

      public:
        posstats();
        ~posstats();

        /** Indicates the maximum profit over the position's lifespan */
        float pnl_peak;

        /** tells how many times position's vstop was increased */
        int vstop_increments;

        /** tells how many times position's vstop was decreased */
        int vstop_decrements;

        /** tells how many times position's vlimit was increased */
        int vlimit_increments;

        /** tells how many times position's vlimit was decreased */
        int vlimit_decrements;

    };

    /**
     * position class aims to store and manipulate the positions open by 
     * the user's broker. 
     */
    class position {

      public:

        /** position class basic constructor */
      	position();

      	/** position destructor */
      	~position();

        /** Sets new vstop value for position object. 
         *  @param new_vstop new vtop value
         *  @return void.
         */
        void set_vstop(int new_vstop);

        /** Sets new vlimit value for position object. 
         *  @param new_vlimit new vlimit value.
         *  @return void.
         */
        void set_vlimit(int new_vlimit);

        /** broker's ticket id */
        std::string ticket_id;

        /** position's asset name, as referenced in quotek */
        std::string asset_name;

        /** position's asset_id, as referenced by broker */
        std::string asset_id;

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

      	/** vlimit is a virtual limit */
      	int vlimit;

    };

  }
}

#endif
