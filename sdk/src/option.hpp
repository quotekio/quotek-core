/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef OPTION_HPP
#define OPTION_HPP

#include <string>

namespace quotek {

  namespace quant {
        
    /** Option is a class meant to modelize the behaviour of options,
     *  for instance in the context of black-sholes modeling.
     *  Please Note that this class is good for vanilla options only,
     *  if you need something more exotic the best way to do is to create
     *  a new class which will inherit from it. */

    class option {

      public:

        /** Option first constructor */
      	option();

        /** Option second constructor */
        option(int type,
        	   std::string underlying,
        	   float price,
        	   float strike,
        	   long expires);

      	/** option destructor */
      	~option();

        /** Underlying contains the asset_id of the option's underlying */
        std::string underlying;

        /** gives the option expiration, in years */ 
        long expires;

        /** price stores the current price of the modelized option */
        float price;

        /** gives the option's strike */
      	float strike;

        /** tells wether the option is a put or a call */
      	int type;

        /** returns wether the option is IN or OUT of the money */
      	bool is_in_money();

        static const int european_call = 0x00;
        static const int european_put = 0x01;

        static const int us_call = 0x10;
        static const int us_put = 0x11;

      private:

      	bool in_money;



    };
  }
}

#endif
