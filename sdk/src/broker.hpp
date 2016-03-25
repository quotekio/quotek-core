/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef BROKER_HPP
#define BROKER_HPP

#include <string>
#include <sstream>
#include "strategy.hpp"

namespace quotek {

/**
 * broker class is meant to embedd all the methods that interract with the broker 
 * configured by the user.
 */

  class broker {

    public:
 
     /**
      * Broker object constructor
      * @param strategy back link to parent strategy object
      * @return new broker object.
      */
     broker(strategy* s);

     /**
      * buy sends a buy order to the broker. it will then open a long position having the caracteristics defined
      * in the arguments.
      * @param asset name of the asset to take position on.
      * @param quantity Number of contracts to take for the position.
      * @param stop Stop-loss security thereshold, in BPS.
      * @param limit Profit thereshold before close, in BPS.
      */
     void buy(std::string asset,
              int quantity,
              int stop,
              int limit);

     /**
      * buy_hedged sends a buy order to the broker along with a sell order for the another asset.
      * quantity, stop and limits are the same for the long position and the hedged one.
      * @param asset name of the asset to take position on.
      * @param hedged_asset name of the asset to take position on for hedging.
      * @param quantity Number of contracts to take for the position.
      * @param stop Stop-loss security thereshold, in BPS.
      * @param limit Profit thereshold before close, in BPS.
      */
     void buy_hedged(std::string asset,
                     std::string hedged_asset,
                     int quantity,
                     int stop,
                     int limit);

      /**
       * same than buy_hedged but here you can choose the quantity, stop and limit for hedged asset.
       */
      void buy_hedged_asym(std::string asset,
                           int quantity,
                           int stop,
                           int limit,
                           std::string hedged_asset,
                           int hedged_quantity,
                           int hedged_stop,
                           int hedged_limit
                           );
     /**
      * sell sends a sell order to the broker. It will then open a short position having the caracteristics
      * defined in the arguments.
      * @param asset name of the asset to take position on.
      * @param quantity Number of contracts to take for the position.
      * @param stop Stop-loss security thereshold, in BPS.
      * @param limit Profit thereshold before close, in BPS.
      */
     void sell(std::string asset,
                int quantity,
                int stop,
                int limit);

     /** smartbuy will place a long trade where stop and limit will be automatically computed according to: 
          - a risk reward ratio provided by the user
          - money management rules defined in the configuration 
     */
     void smartbuy(std::string asset, int expexted_risk_reward);

     /** smartsell will place a short trade where stop and limit will be automatically computed according to: 
          - a risk reward ratio provided by the user.
          - money management rules defined in the configuration.
     */
     void smartsell(std::string asset, int expexted_risk_reward);
     
     /**
      * close_position allows to close a position given its ticket id given by the broker.
      * @param ticket_id id of the position ticket provided by the broker.
      */
     void close_position(std::string ticket_id);

    private:
      
      strategy* s;

  };
}


#endif