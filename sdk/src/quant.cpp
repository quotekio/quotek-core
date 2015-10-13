/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "quant.hpp"
#include <iostream>

namespace quotek {

  namespace quant {

    bool above(quotek::data::records& recs, 
    	         float value,
    	         float thereshold) {

      int nb_above = 0;
      float computed_thold;

      for(int i=0;i<recs.size();i++) {
        if (value > recs[i].value ) nb_above++;
      }

      computed_thold =   (float) nb_above / recs.size() ; 
      if ( computed_thold >= thereshold) return true;
      return false;

    }

    bool crosses(quotek::data::records& recs1,
                             quotek::data::records& recs2) {

      if (recs1.size() < 2 || recs2.size() < 2) return false;
      
      for (int i=1;i<recs1.size();i++) {

        if ( recs1[i-1].value >= recs2[i-1].value && recs1[i].value <= recs2[i].value ) return true;
        else if ( recs2[i-1].value >= recs1[i-1].value && recs2[i].value <= recs1[i].value ) return true;
      }
      return false;
    }

    float min(quotek::data::records& recs) {

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


    float max(quotek::data::records& recs) {
      
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

    float average(quotek::data::records& recs) {
      float sum = 0;
      for(int i=0;i<recs.size();i++) {
        sum += recs[i].value;
      }
      return sum / recs.size();
    }
    
    float weighted_average(quotek::data::records& recs,
                           std::vector<int>& weights) {

      float sum = 0;

      //computes weights sum
      int weights_sum = 0;
      for (int i=0;i<weights.size();i++) {
        weights_sum+= weights[i];
      }

      for(int i=0;i<recs.size();i++) {
        sum += recs[i].value * weights[i];
      }

      return sum / weights_sum;

    }


    float variance(quotek::data::records& recs,
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

    float variance_q(quotek::data::records& recs, 
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
    
    float covariance(quotek::data::records& recs1,
    	             quotek::data::records& recs2) {

      float covariance= 0;
      float v1_a = average(recs1);
      float v2_a = average(recs2);
      for(int i=0;i<recs1.size();i++) {
         covariance += (recs1[i].value - v1_a) * (recs2[i].value - v2_a );    
      }

      covariance /= recs1.size();
      return covariance;
    }

    float covariance_q(quotek::data::records& recs1,
    	               quotek::data::records& recs2,
    	               float avg1,
    	               float avg2) {

      float covariance = 0;
      for(int i=0;i<recs1.size();i++) {
         covariance += (recs1[i].value - avg1) * (recs2[i].value - avg2 );    
      }

      covariance /= recs1.size();

      return covariance;
    }

    float standard_deviation(quotek::data::records& recs, bool sample) {
      return (float) sqrt(variance(recs,sample));
    }

    float percent_delta(quotek::data::records& recs) {
      if (recs.size() == 0) return 0;
      float v0 = recs[0].value;
      float v1 = recs[recs.size()-1].value;
      return ( (v1 - v0) / v0 ) * 100 ;
    }

   
    std::vector<float> moving_average(quotek::data::records& recs,
                                      int periods) {
    
      std::vector<float> result;
      
      //dataset is too short or periods invalid
      if ( recs.size() < periods || periods < 2 ) return result;

      for(int i = 0 ; i < recs.size() - periods + 1 ;i++) {

        quotek::data::records tmprec = recs.extract(i,periods);
        float avg = average(tmprec);
        result.push_back(avg);

      }

      return result;

    }

    std::vector<float> exponential_moving_average(quotek::data::records& recs, 
                                                  int periods) {

      quotek::data::records v_ema_first;
      std::vector<float> result;
      //dataset is too short or periods invalid
      if ( recs.size() < periods || periods < 2 ) return result;

      float k =  2.0 / ( periods + 1 );      

      int start = (periods - 1) / 2  + (periods - 1) % 2 ; 

      //compute first EMA
      v_ema_first = recs.extract(0, start );

      float ema_first = average(v_ema_first);
      result.push_back(ema_first);
            
      for(int i = start; i< recs.size() ; i++) {

        float prev_ema = result[result.size()-1];
        if ( start >= 2  ) result.push_back( recs[i-1].value * k + prev_ema * ( 1- k) );
        else result.push_back( recs[i].value * k + prev_ema * ( 1- k) );
      }
    
      return result;

    }


    std::vector<float> weighted_moving_average(quotek::data::records& recs, 
                                                  int periods) {

      std::vector<float> result;
      
      //dataset is too short or periods invalid
      if ( recs.size() < periods || periods < 2 ) return result;

      //computes weights vector
      std::vector<int> weight_vector;
      for (int i=1;i<=periods;i++) {
        weight_vector.emplace_back(i);
      }

      for(int i = 0 ; i < recs.size() - periods + 1 ;i++) {

        quotek::data::records tmprec = recs.extract(i,periods);
        float avg = weighted_average(tmprec,weight_vector);
        result.push_back(avg);
      }
      return result;

    }

    void linear_regression(quotek::data::records& recs, std::vector<float>& result) {

      affine af = linear_regression(recs);
      
      for (int i=0;i<recs.size();i++) {
        result.push_back( af.a * recs[i].timestamp + af.b );
      }

    }
    
    affine linear_regression(quotek::data::records& recs) {

        affine result;

        quotek::data::records T_series;

        for (int i=1;i<recs.size()+1;i++) {
          quotek::data::record r(0,i,0);
          T_series.append(r);
        }

        float T_average = average(T_series);
        float V_average = average(recs);

        result.a = covariance(T_series, recs) / variance(T_series, false);
        result.b = V_average - ( result.a * T_average ) ; 

        return result;
    }

    trend_p trend_percentages(quotek::data::records& recs) {

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

    float normal_cumulative_distribution(float value) {
      return 0.5 * erfc(-value * M_SQRT1_2);
    }

    float standard_normal_distribution(float value) {
      return (1.0/sqrt(2.0* 3.141592653589793238462643 )) * exp(-0.5* pow(value,2) );
    }

  }

}
