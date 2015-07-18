/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "quant.hpp"
#include <iostream>

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
        variance += pow( recs[i].value - mean, 2);    
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

      float variance = 0;
      for(int i=0;i<recs.size();i++) {
        variance += pow( recs[i].value - average,2);    
      }

      if (sampled) {
        variance /= ( recs.size() -1 ); 
      }

      else {
        variance /= recs.size();
      }

      return variance;
    }
    
    float covariance(std::vector<quotek::data::record>& recs1,
    	             std::vector<quotek::data::record>& recs2) {

      float covariance= 0;
      float v1_a = average(recs1);
      float v2_a = average(recs2);
      for(int i=0;i<recs1.size();i++) {
         covariance += (recs1[i].value - v1_a) * (recs2[i].value - v2_a );    
      }

      covariance /= recs1.size();
      return covariance;
    }

    float covariance_q(std::vector<quotek::data::record>& recs1,
    	               std::vector<quotek::data::record>& recs2,
    	               float avg1,
    	               float avg2) {

      float covariance = 0;
      for(int i=0;i<recs1.size();i++) {
         covariance += (recs1[i].value - avg1) * (recs2[i].value - avg2 );    
      }

      covariance /= recs1.size();

      return covariance;
    }

    float standard_deviation(std::vector<quotek::data::record>& recs, bool sample) {
      return (float) sqrt(variance(recs,sample));
    }

    float percent_delta(std::vector<quotek::data::record>& recs) {
      float v0 = recs[0].value;
      float v1 = recs[recs.size()-1].value;
      return ( (v1 - v0) / v0 ) * 100 ;
    }

   
    std::vector<float> moving_average(std::vector<quotek::data::record>& recs,
                                      int periods) {
    
      std::vector<float> result;
      
      //dataset is too short or periods invalid
      if ( recs.size() < periods || periods < 2 ) return result;

      for(int i = 0 ; i < recs.size() - periods + 1 ;i++) {

        std::vector<quotek::data::record> tmprec = quotek::data::record::extract(recs, i,periods);
        float avg = average(tmprec);
        result.push_back(avg);

      }

      return result;

    }

    std::vector<float> exponential_moving_average(std::vector<quotek::data::record>& recs, 
                                                  int periods) {

      std::vector<float> result;
      //dataset is too short or periods invalid
      if ( recs.size() < periods || periods < 2 ) return result;

      float k =  2.0 / ( periods + 1 );      

      int start = (periods - 1) / 2 ;

      int start_modulo = (periods - 1) % 2;

      //compute first EMA
      std::vector<quotek::data::record> v_ema_first = quotek::data::record::extract(recs,0, start + start_modulo );

      if (start > 0) start_modulo = 0;
      
      float ema_first = average(v_ema_first);
      result.push_back(ema_first);

      using namespace std;
      cout << "K:" << k << endl << "START:" << start << endl << "START_MODULO:" << start_modulo << endl << "EMA_FIRST:" << ema_first << endl;

      for(int i = start + start_modulo ; i< recs.size() ; i++) {

        float prev_ema = result[result.size()-1];
        result.push_back( recs[i].value * k + prev_ema * ( 1- k) );

      }

      return result;

    }

    void linear_regression(std::vector<quotek::data::record>& recs, std::vector<float>& result) {

      affine af = linear_regression(recs);
      
      for (int i=0;i<recs.size();i++) {
        result.push_back( af.a * recs[i].timestamp + af.b );
      }

    }
    
    affine linear_regression(std::vector<quotek::data::record>& recs) {

        affine result;

        std::vector<quotek::data::record> T_series;

        for (int i=1;i<recs.size()+1;i++) {
          T_series.emplace_back( quotek::data::record(0, i, 0) );
        }

        float T_average = average(T_series);
        float V_average = average(recs);

        result.a = covariance(T_series, recs) / variance(T_series, false);
        result.b = V_average - ( result.a * T_average ) ; 

        return result;
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

      result.bull /= recs.size() -1 ;
      result.bull *= 100;

      result.bear /= recs.size() - 1;
      result.bear *= 100;
      
      result.neutral /= recs.size() -1 ;
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
