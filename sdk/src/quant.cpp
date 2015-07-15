/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "quant.hpp"

namespace quotek {

  namespace quant {

    bool above(std::vector<quotek::data::record>& recs, 
    	         float value,
    	         float thereshold) {

      int nb_above = 0;
      float computed_thold;

      for(int i=0;i<recs.size();i++) {
        if (value > recs[i].value ) nb_above++;
      }

      computed_thold = (float) (nb_above / recs.size() ); 
      if ( computed_thold >= thereshold) return true;
      return false;

    }


    float min(std::vector<quotek::data::record>& recs) {

      float min = 300000000;
      float cur = 0;

      for(int i=0;i<recs.size();i++) {
         cur = recs[i].value;
         if ( cur < min ) {
           min = cur;
         }
      }
      return min;
    }


    float max(std::vector<quotek::data::record>& recs) {
      
      float max = -300000000;
      float cur = 0;

      for(int i=0;i<recs.size();i++) {
         cur = recs[i].value;
         if ( cur > max ) {
           max = cur;
         }
      }
      return max;
    }

    float average(std::vector<quotek::data::record>& recs) {
      float sum = 0;
      for(int i=0;i<recs.size();i++) {
        sum += recs[i].value;
      }
      return sum / recs.size();
    }
    

    float variance(std::vector<quotek::data::record>& recs,
    	           bool sampled) {

      float mean = average(recs);
      float variance = 0;
      for(int i=0;i<recs.size();i++) {
        variance += pow( recs[i].value - mean,2);    
      }

      if (sampled) {
        variance /= ( recs.size() -1 ); 
      }

      else {
        variance /= recs.size();
      }
      return variance;
    }

    float variance_q(std::vector<quotek::data::record>& recs, 
    	             bool sampled,
    	             float average) {

    }
    
    
    float covariance(std::vector<quotek::data::record>& recs1,
    	             std::vector<quotek::data::record>& recs2) {

    }


    float covariance_q(std::vector<quotek::data::record>& recs1,
    	               std::vector<quotek::data::record>& recs2,
    	               float avg1,
    	               float avg2) {

    }

    float standard_deviation(std::vector<quotek::data::record>& recs) {
      return (float) sqrt(variance(recs,0));
    }

    float percent_delta(std::vector<quotek::data::record>& recs) {
      float v0 = recs[0].value;
      float v1 = recs[recs.size()-1].value;
      return ( (v1 - v0) / v0 ) * 100 ;
    }

   
    std::vector<float> moving_average(std::vector<quotek::data::record>& recs,
                                      int periods) {
    
      std::vector<float> result;
      return result;

    }

    std::vector<float> exponential_moving_average(std::vector<quotek::data::record>& recs, 
                                                  int periods) {

      std::vector<float> result;
      return result;

    }

    std::vector<float> linear_regression(std::vector<quotek::data::record>& recs) {

    }

    affine linear_regression_affine(std::vector<quotek::data::record>& recs) {

    }

    trend_p trend_percentages(std::vector<quotek::data::record>& recs) {

      trend_p result;
      result.bull = 0;
      result.bear = 0;
      result.neutral = 0;

      for (int i=0;i<recs.size()-1;i++) {

        if ( recs[i].value < recs[i+1].value ) result.bull++;
        else if ( recs[i].value > recs[i+1].value ) result.bear ++;
        else result.neutral++;
      }

      result.bull /= recs.size();
      result.bull *= 100;

      result.bear /= recs.size();
      result.bear *= 100;
      
      result.neutral /= recs.size();
      result.neutral *= 100;
      
      return result;

    }

    float risk_reward_ratio(float expected_profit,  
                            int stop, 
                            int leverage) {

      float max_loss = leverage * stop; 
      return expected_profit / max_loss ;

    }


  }


}
