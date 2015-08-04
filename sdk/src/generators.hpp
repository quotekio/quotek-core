/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include <random>
#include <string>
#include "record.hpp"

namespace quotek {

  namespace rand {

    /** The quotek rand generators namespace contains c++11 distributions wrappers 
     *  which add an extra abstraction layer and also fit the quotek dataset format.
     *  It mainly allows to generate randoml value datasets having different kinds of distributions,
     *  which can prove really useful for financial modeling. 
     */
  	namespace generators {

      /** Generates a set of random values which are uniformly distributed.
       *  @param size number of values we want in the dataset.
       *  @param min Minimum value that must be present inside result dataset.
       *  @param max Maximum value that must be present inside result dataset.
       *  @return the generated dataset.
       */
      std::vector<quotek::data::record> uniform(int size, float min, float max);

      /** Generates a set of random values which are normally distributed. 
       *  (probaly the most useful to modelize asset prices)
       *  @param size number of values we want in the dataset.
       *  @param mean average value we want for our generator.
       *  @param sigma wanted standard deviation.
       *  @return the generated dataset.
       */
       std::vector<quotek::data::record> normal(int size, float mean, float sigma);

       /** Generates a set of random values which are log-normally distributed. 
        *  @param size number of values we want in the dataset.
        *  @param mean mean we want to use for the underlying normal distribution. value we want for our generator.
        *  @param sigma standard deviation we want to use for the underlying normal distribution.
        *  @return the generated dataset.
        */
        std::vector<quotek::data::record> lognormal(int size, float mean, float sigma);

        /** Generates a set of random values which are distributed using a binomial law. 
         *  @param size number of values we want in the dataset.
         *  @param experiments the number of tries for a number
         *  @param success_probability probability that each experiment succeeds .
         *  @return the generated dataset.
         */
        std::vector<quotek::data::record> binomial(int size, int experiments , float success_probability );

        /** Generates a set of random values which are distributed using a poisson law. 
         *  @param size number of values we want in the dataset.
         *  @param average_occurence number of times an event must occur on everage for the series.
         *  @return the generated dataset.
         */
        std::vector<quotek::data::record> poisson(int size, int average_occurence);


  	}
  }
}

#endif
