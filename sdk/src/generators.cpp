/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "generators.hpp"

namespace quotek {

  namespace rand {

  	namespace generators {

      std::vector<quotek::data::record> uniform(int size, float min, float max) {

        std::vector<quotek::data::record> result;

        std::uniform_real_distribution<float> unif(min,max);
        std::default_random_engine re;

        for (int i=0;i<size;i++) {
          result.emplace_back(quotek::data::record(0,unif(re),0));
        }
        return result;
      }

      std::vector<quotek::data::record> normal(int size, float mean, float sigma) {

        std::vector<quotek::data::record> result;

        std::normal_distribution<float> norm(mean,sigma);
        std::default_random_engine re;

        for (int i=0;i<size;i++) {
          result.emplace_back(quotek::data::record(0,norm(re),0));
        }
        return result;
      }

      std::vector<quotek::data::record> lognormal(int size, 
                                                 float mean, 
                                                 float sigma) {

        std::vector<quotek::data::record> result;

        std::lognormal_distribution<float> lnorm(mean,sigma);
        std::default_random_engine re;

        for (int i=0;i<size;i++) {
          result.emplace_back(quotek::data::record(0,lnorm(re),0));
        }
        return result;
      }

      std::vector<quotek::data::record> binomial(int size, 
                                                 int experiments, 
                                                 float success_probability) {

        std::vector<quotek::data::record> result;
        std::normal_distribution<int> binom(experiments,success_probability);
        std::default_random_engine re;

        for (int i=0;i<size;i++) {
          result.emplace_back(quotek::data::record(0,(float) binom(re),0));
        }
        return result;
      }

      std::vector<quotek::data::record> poisson(int size, int average_occurence ) {

        std::vector<quotek::data::record> result;
        std::poisson_distribution<int> poiss(average_occurence);
        std::default_random_engine re;

        for (int i=0;i<size;i++) {
          result.emplace_back(quotek::data::record(0,(float) poiss(re),0));
        }
        return result;
      }
  	}
  }
}
