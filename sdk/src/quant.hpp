/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef QUANT_HPP
#define QUANT_HPP

#include <string>
#include <vector>
#include "any.hpp"
#include "record.hpp"
#include "math.h"

namespace quotek {

  /** the quant namespace contains all the required function to work on asset value 
  * from a quantitative perspective. */
  namespace quant {

    //Functions shortcuts
    #define AVG average
    #define SMA moving_average
    #define EMA exponential_moving_average
    #define SD standard_deviation
    #define SIGMA standard_deviation
    #define SLR linear_regression
    #define SLR_AFFINE linear_regression_affine
    #define RRR risk_reward_ratio

  	/** This structure aims to store affine equations parameters. */
  	typedef struct affine {
  	  float a;
  	  float b;
  	} affine;

  	/** This structure stores trend percentage analysis. */
  	typedef struct trend_p {
  	  float bull;
  	  float bear;
  	  float neutral;
  	} trend_p;


    /**
     * above tells if the values contained in the provided dataset are mostly above 
     * or below value. accuracy will depend on provided thereshold (between 0 and 1) , 
     * 1 meaning that all the values of dataset must be higher than value to return true, 0.5 meaning that
     * half the values must be higher.
     */     
    bool above(std::vector<quotek::data::record>& recs, 
    	      float value,
    	      float thereshold);

    /**
     * min() returns the smallest value contained in the provided
     * recs dataset.
     */
    float min(std::vector<quotek::data::record>& recs);

    
    /**
     * max() return the largest value contained in the provided
     * recs dataset.
     */
    float max(std::vector<quotek::data::record>& recs);

    /**
     * This function returns an average of the values contained in
     * the provided recs dataset. 
     */
    float average(std::vector<quotek::data::record>& recs);
    
    /**
     * Returns the variance of the values contained in the provided recs dataset.
     */
    float variance(std::vector<quotek::data::record>& recs,
    	           bool sampled);

    /**
     * variance_q is a performance-shaped function that will compute variance of the values 
     * for the provided recs dataset but also provided that you already know the average of the values from
     * previous compute.
     */

    float variance_q(std::vector<quotek::data::record>& recs, 
    	             bool sampled,
    	             float average);
    
    /**
     * Computes covariance of values between two datasets recs1 and recs2.
     */
    float covariance(std::vector<quotek::data::record>& recs1,
    	             std::vector<quotek::data::record>& recs2);


    /**
     * Quick version of covariance, provided that you already know the averages of the 2 datasets.
     */
    float covariance_q(std::vector<quotek::data::record>& recs1,
    	               std::vector<quotek::data::record>& recs2,
    	               float avg1,
    	               float avg2);

    /**
     * Computes the standard deviation for the values contained in the recs dataset.
     */
    float standard_deviation(std::vector<quotek::data::record>& recs);

    /**
     * Computes the delta in percent between first and last value of the recs dataset.
     */
    float percent_delta(std::vector<quotek::data::record>& recs);

    /**
     * Computes the simple moving average of the provided dataset on n periods (in seconds)
     * eg if you want to compute an H1 SMA, use moving_average()
     */
    std::vector<float> moving_average(std::vector<quotek::data::record>& recs,
                                      int  periods);

    /**
     * Computes the exponential moving average of the provided dataset on n period (in seconds)
     */
    std::vector<float> exponential_moving_average(std::vector<quotek::data::record>& recs, 
                                                  int periods);

    /**
     * linear_regression computes the linear regression of the provided dataset
     */
    std::vector<float> linear_regression(std::vector<quotek::data::record>& recs);


    /**
     * Computes the linear regretion of the values contained in the dataset and
     * gives the result as an affine structure such as the line equation is y = affine.a * x + affine.b
     */
    affine linear_regression_affine(std::vector<quotek::data::record>& recs);

    /** 
     * Computes the bull, bear and neutral ratios for the provided dataset.
     * ratios are provided in percentage.
     */
    trend_p trend_percentages(std::vector<quotek::data::record>& recs);

    /**
     * This function returns the risk/reward ratio for a potential trade having 
     * the provided caracteristics.
     */
    float risk_reward_ratio(float expected_profit,  
                            int stop, 
                            int leverage);


  }
}

#endif
