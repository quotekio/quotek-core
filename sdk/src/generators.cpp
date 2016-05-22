/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#include "generators.hpp"

namespace quotek {

  namespace rand {

  	namespace generators {

      quotek::data::records uniform(int size, float min, float max) {

        quotek::data::records result;

        std::uniform_real_distribution<float> unif(min,max);
        std::default_random_engine re(std::random_device{}());

        for (int i=0;i<size;i++) {
          quotek::data::record r(0,unif(re),0);
          result.append(r);
        }
        return result;
      }

      quotek::data::records normal(int size, float mean, float sigma) {

        quotek::data::records result;

        std::normal_distribution<float> norm(mean,sigma);
        std::default_random_engine re(std::random_device{}());

        for (int i=0;i<size;i++) {
          quotek::data::record r(0,norm(re),0);
          result.append(r);
        }
        return result;
      }

      quotek::data::records lognormal(int size, 
                                                 float mean, 
                                                 float sigma) {

        quotek::data::records result;

        std::lognormal_distribution<float> lnorm(mean,sigma);
        std::default_random_engine re(std::random_device{}());

        for (int i=0;i<size;i++) {
          quotek::data::record r(0,lnorm(re),0);
          result.append(r);
        }
        return result;
      }

      quotek::data::records binomial(int size, 
                                                 int experiments, 
                                                 float success_probability) {

        quotek::data::records result;
        std::binomial_distribution<int> binom(experiments,success_probability);
        std::default_random_engine re(std::random_device{}());

        for (int i=0;i<size;i++) {
          quotek::data::record r(0,(float) binom(re),0);
          result.append(r);
        }
        return result;
      }

      quotek::data::records poisson(int size, int average_occurence ) {

        quotek::data::records result;
        std::poisson_distribution<int> poiss(average_occurence);
        std::default_random_engine re(std::random_device{}());

        for (int i=0;i<size;i++) {
          quotek::data::record r(0,(float) poiss(re),0);
          result.append(r);
        }
        return result;
      }
  	}
  }
}
