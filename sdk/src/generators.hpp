/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <string>

namespace quotek {

  namespace rand {

  	namespace generators {

      /** Generates a set of random values which are uniformly distributed.
       *  @param size number of values we want in the dataset.
       *  @param mean average value we want for our generator.
       *  @param wanted standard deviation.
       *  @return the generated dataset.
       */

      std::vector<records> uniform(int size, float mean, float sigma);

      /** Generates a set of random values which are normally distributed. 
       *  (probaly the most useful to modelize asset prices)
       *  @param size number of values we want in the dataset.
       *  @param mean average value we want for our generator.
       *  @param sigma wanted standard deviation.
       *  @return the generated dataset.
       */

       std::vector<records> normal(int size, float mean, float sigma);

       /** Generates a set of random values which are log-normally distributed. 
        *  (probaly the most useful to modelize asset prices)
        *  @param size number of values we want in the dataset.
        *  @param mean average value we want for our generator.
        *  @param sigma wanted standard deviation.
        *  @return the generated dataset.
        */
        std::vector<records> lognormal(int size, float mean, float sigma);


  	}
  }
}

#endif
