/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <sstream>

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

        /** Returns the pos stats object under the form of a json-formated string */
        std::string str();

        //Maybe better than str()
        /*operator std::string () const {
          return "SomeClassStringRepresentation";
        }*/
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

        /** vshift allows to shift vstop/limit values by number provided as argument.
         *  If position has a stop of 30 BPS and a limit of 50, pos.vshift(10) will set vstop to 20 and limit to 60.
         *  On the other side, pos.vshift(-10) will set vstop at 40 BPS and limit of 40.
         *  @param offset value to increase/decrease. */
        void vshift(int offset);

        /** Retrieves vstop private value 
         *  @return vstop value
         */
        float get_vstop();

        /** Retrieves vlimit private value 
         *  @return vlimit value;
         */
        float get_vlimit();
        
        /** returns position as a json-formated string */
        std::string str();


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
      	
      	/** position stop-loss. */
      	float stop;

        /** position limit. */
      	float limit;
        
        /** position open date, as epoch timestamp */
        long open_date;
 
        /** date at which the position has been shut */
        long close_date;

        posstats* stats;

        /** Identifier is a special string meant to identify the position taker. 
         *  It is not mandatory if you don't need to know who/what actually opened the position. */
        std::string identifier;

      private:

      	/** vstop is a virtual stop-loss */
      	int vstop;

      	/** vlimit is a virtual limit */
      	int vlimit;

    };

  }
}

#endif
