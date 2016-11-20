/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#include "quant.hpp"
#include <iostream>
#include "eigen3/unsupported/Eigen/FFT"

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

    bool cross(quotek::data::records& recs1,
                             quotek::data::records& recs2) {

      quotek::data::records r1;
      quotek::data::records r2;

      if (recs1.size() < 2 || recs2.size() < 2) return false;

      //We put the time-series at the same size
      if ( recs1.size() > recs2.size() ) {
        r1 = recs1.extract(recs2.size());
        r2 = recs2;
      }
      else {
        r1 = recs1;
        r2 = recs2.extract(recs1.size());
      }
      //

      for (int i= r1.size() - 1 ;i > 0 ; i--) {
        if ( r1[i-1].value >= r2[i-1].value && r1[i].value <= r2[i].value ) return true;
        else if ( r2[i-1].value >= r1[i-1].value && r2[i].value <= r1[i].value ) return true;
      }

      return false;
    }

    int cross_ex(quotek::data::records& recs1,
              quotek::data::records& recs2) {

      quotek::data::records r1;
      quotek::data::records r2;

      if (recs1.size() < 2 || recs2.size() < 2) return false;

      //We put the time-series at the same size
      if ( recs1.size() > recs2.size() ) {
        r1 = recs1.extract(recs2.size());
        r2 = recs2;
      }
      else {
        r1 = recs1;
        r2 = recs2.extract(recs1.size());
      }
      //

      for (int i= r1.size() - 1 ;i > 0 ; i--) {

        if ( r1[i-1].value >= r2[i-1].value && r1[i].value <= r2[i].value ) {
          //r2 is on top
          return 2;
        }
        else if ( r2[i-1].value >= r1[i-1].value && r2[i].value <= r1[i].value ) {
          //r1 is on top
          return 1;
        }
      }
      
      return 0;
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

    quotek::data::records differenciate(quotek::data::records& recs, int order) {
 
      quotek::data::records buff[order + 1];
      
      buff[0] = recs;

      for (int i = 1; i < order + 1 ;i++) {
        for ( int j = 1; j < buff[i-1].size(); j++ ) {
          float val = buff[i-1][j].value - buff[i-1][j-1].value;
          buff[i].append( 0, val , 0 );
        }
      }
      return buff[order];
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

    std::vector<float> triple_exponential_moving_average(quotek::data::records& recs,
                                                         int periods) {

      std::vector<float> result;

      //ema
      std::vector<float> ema1 = exponential_moving_average(recs, periods);
      quotek::data::records recs1 = quotek::data::records(ema1);

      //ema of ema
      std::vector<float> ema2 = exponential_moving_average(recs1, periods);
      quotek::data::records recs2 = quotek::data::records(ema2);

      //ema of ema of ema
      std::vector<float> ema3 = exponential_moving_average(recs2, periods);
      quotek::data::records recs3 = quotek::data::records(ema3);

      //Not totally sure of this ! Let's see if it goes smooth
      int ema1_diff = ema1.size() - ema3.size();
      int ema2_diff = ema2.size() - ema3.size();


      for (int i=0; i< ema3.size(); i++ ) {
        //computes (3xEMA)-(3xEMAofEMA)+(EMAof EMAofEMA)
        result.emplace_back( 3 * ema1[i + ema1_diff ] - 3 * ema2[i + ema2_diff ] + ema3[i] );
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

    size_t fft_next_good_size(size_t N) { 
      while (true) {
        size_t m = N;
        while((m % 2) == 0) m /= 2;
        while((m % 3) == 0) m /= 3;
        while((m % 5) == 0) m /= 5;
        if (m <= 1)
          return N; 
          N++;
      }
    }
       
    void autocorrelation_(const std::vector<float>& y,
                          float y_mean,
                          std::vector<float>& ac,
                          Eigen::FFT<float>& fft) {

        using std::vector;
        using std::complex;

        size_t N = y.size();
        size_t M = fft_next_good_size(N); 
        size_t Mt2 = 2 * M;


        vector<complex<float> > freqvec;
        
        // centered_signal = y-mean(y) followed by N zeroes
        vector<float> centered_signal(y);
        centered_signal.insert(centered_signal.end(),Mt2-N,0.0);
        
        for (size_t i = 0; i < N; i++)
          centered_signal[i] -= y_mean;
        
        fft.fwd(freqvec,centered_signal);
        for (size_t i = 0; i < Mt2; ++i)
          freqvec[i] = complex<float>(norm(freqvec[i]), 0.0);
        
        fft.inv(ac,freqvec);
        ac.resize(N);


        for (size_t i = 0; i < N; ++i) {
          ac[i] /= (N - i); 
        } 
        float var = ac[0];      
        for (size_t i = 0; i < N; ++i)
          ac[i] /= var;
    }

    void autocorrelation_(const std::vector<float>& y,
                           float y_mean,
                           std::vector<float>& ac) {
        Eigen::FFT<float> fft;
        return autocorrelation_(y,y_mean,ac,fft);
    }


    std::vector<float> autocorrelation(quotek::data::records recs) {

      std::vector<float> result;

      float y_mean = average(recs);

      autocorrelation_(recs.export_values(), y_mean, result);
      return result;

    }

  }

}
