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
    #define PLR polynomial_regression
    #define RRR risk_reward_ratio

  	/** This structure aims to store slope-intercept equations parameters. */
  	typedef struct affine {
      /** slope */
  	  float a;
      /** intercept */
  	  float b;

  	} affine;

    /** trinom is a struct that aims to store 2nd degree polynom parameters. */
    typedef struct trinom {
      //x2 coef
      float a;
      //x coef
      float b;
      //offset
      float c;
    } trinom;


  	/** This structure stores trend percentage analysis. */
  	typedef struct trend_p {
      /** stores bull percentage of a dataset */
  	  float bull;
      /** stores bear percentage of a dataset. */
  	  float bear;
      /** stores neutral percentage of a dataset. */
  	  float neutral;
  	} trend_p;


    /**
     * above tells if the values contained in the provided dataset are mostly above 
     * or below value. accuracy will depend on provided thereshold (between 0 and 1) , 
     * 1 meaning that all the values of dataset must be higher than value to return true, 0.5 meaning that
     * half the values must be higher.
     * @param recs dataset to work on.
     * @param value value to test dataset against.
     * @param thereshold integer in [0,1] that indicates the percentage of value in the dataset that must be above value.
     * @return true if dataset is above value, false otherwise.
     */     
    bool above(std::vector<quotek::data::record>& recs, 
    	      float value,
    	      float thereshold);

    /**
     * min() returns the smallest value contained in the provided
     * recs dataset to work on.
     * @param recs dataset to work on.
     * @return the smallest value stored in recs dataset.
     */

    float min(std::vector<quotek::data::record>& recs);

    
    /**
     * max() return the largest value contained in the provided
     * @param recs dataset to work on.
     * @return the largest value stored in recs dataset.
     */
    float max(std::vector<quotek::data::record>& recs);

    /**
     * This function returns an average of the values contained in
     * the provided recs dataset.
     * @param recs dataset to work on.
     * @return the mean value of the dataset, as a float value.
     */
    float average(std::vector<quotek::data::record>& recs);
    
    /**
     * Returns the variance of the values contained in the provided recs dataset.
     * @param recs dataset to work on.
     * @param sampled when you need to make a variance estimate, elimites bias by dividing by N-1 instead of N.
     * @return variance of the recs dataset, as a float value.
     */
    float variance(std::vector<quotek::data::record>& recs,
    	           bool sampled);

    /**
     * variance_q is a performance-shaped function that will compute variance of the values 
     * for the provided recs dataset but also provided that you already know the average of the values from
     * previous compute.
     * @param recs dataset to work on.
     * @param sampled when you need to make a variance estimate, elimites bias by dividing by N-1 instead of N.
     * @param average pre-computed average of the dataset (for instance coming from previous calculus).
     * @return variance of the recs dataset, as a float value.
     */

    float variance_q(std::vector<quotek::data::record>& recs, 
    	             bool sampled,
    	             float average);
    
    /**
     * Computes covariance of values between two datasets recs1 and recs2.
     * @param recs1 the first dataset to work on.
     * @param recs2 the second dataset to work on.
     * @return the covariance between the 2 provided datasets, as a float.
     */
    float covariance(std::vector<quotek::data::record>& recs1,
    	             std::vector<quotek::data::record>& recs2);

    /**
     * Quick version of covariance, provided that you already know the averages of the 2 datasets.
     * @param recs1 the first dataset to work on.
     * @param recs2 the second dataset to work on.
     * @param average_1 pre-computed average of first dataset.
     * @param average_2 prec-computed average of second dataset.
     * @return the covariance between the 2 provided datasets, as a float.
     */
    float covariance_q(std::vector<quotek::data::record>& recs1,
    	               std::vector<quotek::data::record>& recs2,
    	               float average_1,
    	               float average_2);

    /**
     * Computes the standard deviation for the values contained in the recs dataset.
     * @param recs dataset to work on.
     * @return the standard deviation of the provided dataset, as a float.
     */
    float standard_deviation(std::vector<quotek::data::record>& recs);

    /**
     * Computes the delta in percent between first and last value of the recs dataset.
     * @param recs dataset to work on.
     * @return the variation in percents from the beginning to the end of the dataset.
     */
    float percent_delta(std::vector<quotek::data::record>& recs);

    /**
     * Computes the simple moving average of the provided dataset for n periods.
     * eg if you want to compute an SMA50 for the provided dataset, use moving_average(recs,50)
     * @param recs dataset to work on.
     * @param periods the number of dataset values that must be aggregated together to compute a single point of the SMA.
     * @return a vector of floats containing the graph values of the moving average for the dataset.
     */
    std::vector<float> moving_average(std::vector<quotek::data::record>& recs,
                                      int  periods);

    /**
     * Computes the exponential moving average of the provided dataset for n periods.
     * eg if you want to compute an EMA100 for the provided dataset, use exponential_moving_average(recs,50)
     * @param recs dataset to work on.
     * @param periods the number of dataset values that must be aggregated together to compute a single point of the EMA.
     * @return a vector of floats containing the graph values of the moving average for the dataset.
     */
    std::vector<float> exponential_moving_average(std::vector<quotek::data::record>& recs, 
                                                  int periods);

    /**
     * linear_regression computes the linear regression of the provided dataset.
     * @param recs dataset to work on.
     * @param result reference, to store the result of the linear regression.
     * @return none.
     */
    void linear_regression(std::vector<quotek::data::record>& recs, std::vector<float>& result);
    
    /**
     * Computes the linear regretion of the values contained in the dataset and
     * gives the result as an affine structure such as the line equation is y = affine.a * x + affine.b
     * @param recs dataset to work on.
     * @return affine structure containing slope in tercept values.
     */
    affine linear_regression(std::vector<quotek::data::record>& recs);

    /**
     * polynomial regression computes a polynomia regression from a provided recs dataset.
     * @param recs dataset to work on.
     * @return vector of floats representing the polynomial regression of dataset.
     */
     std::vector<float> polynomial_regression(std::vector<quotek::data::record>& recs);

    /** 
     * Computes the bull, bear and neutral ratios for the provided dataset.
     * ratios are provided in percentage.
     * @param recs dataset to work on.
     * @return a trend_p structure containing the bull, bear and neutral trend percentages for the dataset.
     */
    trend_p trend_percentages(std::vector<quotek::data::record>& recs);

    /**
     * This function returns the risk/reward ratio for a potential trade having the provided caracteristics.
     * @param expected_profit profit in asset's currency expected for a trade.
     * @param stop value in BPS where you intend to place your stop value.
     * @param leverage, exprimed in protit per BPS, you intend to add on your trade.
     */
    float risk_reward_ratio(float expected_profit,  
                            int stop, 
                            int leverage);


  }
}

#endif
