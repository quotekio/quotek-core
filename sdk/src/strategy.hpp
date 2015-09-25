/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <regex>
#include <map>

#include "any.hpp"
#include "cqueue.hpp"
#include "cvector.hpp"
#include "position.hpp"
#include "record.hpp"


/**
* strategy class is one of the most important of quotek adam bot.
* When a strategy is created by an user, it inherits from it.
* strategy class also handles the execution of the strat through the run() method.
*/

class strategy {

public:

/** strategy constructor. */
strategy() {}

/**
 * strategy destructor
 */
~strategy() {}

/**
 * set_env is a function meant to connect the strategy to quotek SaaS environment.
 * @param recs dataset for the concerned asset.
 * @param store shared algo in-memory map.
 * @param portfolio global portfolio containing all the taken positions.
 * @return new strategy object.
 */
void set_env(quotek::data::records* recs,
           std::map<std::string, quotek::data::any>* store,
           quotek::data::cvector<quotek::core::position>*portfolio) {

  this->recs = recs;
  this->store = store;
  this->portfolio = portfolio;  

}

/**
 * order() is a method meant to make direct orders to adam bot.
 * @param order_data string representing the bot order to pass.
 */
 void order(std::string order_data) {
   orders_queue.push(order_data);
 }

/**
* log() is a method that allows the user algorithm to forward log data to adam.
* @param log_string string to log.
*/
void log(std::string log_string) {
  log_queue.push(log_string);
}

/**
 * initialize() is a method meant to set the initial state of a strategy.
 * This method is executed only once, before the first evaluation. 
 */
virtual int initialize() { return 0; }

/**
 * evaluate() method is the very core of the the strategy, this is where 
 * the user's algorithm gets executed, at a pace of once per value tick.
 */
virtual void evaluate() {}

/**
 * tradelife() is here to manage the open positions and decide when to close them, 
 *  or change the ticket (stops, limite, ticket size..) 
 */
virtual void tradelife()  {}

std::vector<strategy*> substrats;

/** state is a variable meant to keep track of the state of the algorithm. it's up to the user to set it accordingly. */
int state;

/** value stores the current value of an asset. */
float value;

/** spread stores the current spread of an asset provided by the broker. */
float spread;

/** Asset name stores the asset for which the strategy instance is running */
std::string asset_name;

/** t stores the current epoch timestamp */
long t;

/** These are the I/O queues used by the strategies to communicate with the other
 *  parts of the robot. These are not meant to be manipulated by user in his algorithms.
 */
quotek::data::cqueue<std::string> log_queue;
quotek::data::cqueue<std::string> orders_queue;

/**
 * recs variable points to the inmem values history of the processed asset.
 */
quotek::data::records* recs;

/**
 * store is a special object meant to share data between strategies threads.
 */
std::map<std::string, quotek::data::any>* store;

/**
 * portfolio contains the current list of running positions.
 */
quotek::data::cvector<quotek::core::position>* portfolio;
/**
 * counters allow to maintain a series of variables to count and keep various steps across ticks.
 */
std::map<std::string, int> counters;

};

//!! strategy import class pointers !!
typedef strategy* create_st();
typedef void destroy_st(strategy*);

#endif