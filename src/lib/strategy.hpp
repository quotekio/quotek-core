#include <regex>
#include <thread>

/**
 * strategy class is one of the most important of quotek adam bot.
 * When a strategy is created by an user, it inherits from it.
 * strategy class also handles the execution of the strat through the run() method.
 */

class strategy {

  public:
  	/**
  	 * strategy constructor.
  	 */
    strategy();

    /**
     * strategy destructor
     */
    ~strategy();

    /**
     * run() method handles the execution of the strat.
     */
    std::thread* run();

    /**
     * initialize() is a method meant to set the initial state of a strategy.
     * This method is executed only once, before the first evaluation. 
     */
    virtual int initialize();

    /**
     * evaluate() method is the very core of the the strategy, this is where 
     * the user's algorithm gets executed, at a pace of once per value tick.
     */
    virtual void evaluate();

    /**
     * tradelife() is here to manage the open positions and decide when to close them, 
     *  or change the ticket (stops, limite, ticket size..) 
     */
    virtual void tradelife();

  private:
    vector<strategy*> substrats;

    /** state is a variable meant to keep track of the state of the algorithm. it's up to the user to set it accordingly. */
    int state;
    
    /** value stores the current value of an asset. */
    float value;

    /** spread stores the current spread of an asset provided by the broker. */
    float spread;

};