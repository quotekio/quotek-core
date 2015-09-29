/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef QUANT_HPP
#define QUANT_HPP

#include <string>
#include <vector>
#include "any.hpp"
#include "record.hpp"
#include "math.h"

#ifdef _WIN64
#define M_SQRT1_2 1.0 / sqrt(2)
#endif

namespace quotek {

  /** the quant namespace contains all the required function to work on asset value 
  * from a quantitative perspective. */
  namespace quant {

    //Functions shortcuts
    #define AVG average
    #define SMA moving_average
    #define EMA exponential_moving_average
    #define WMA weighted_moving_average
    #define SD standard_deviation
    #define SIGMA standard_deviation
    #define SLR linear_regression
    #define OLS linear_regression
    #define PLR polynomial_regression
    #define RRR risk_reward_ratio

    #define N(_a) normal_cumulative_distribution(_a)
    #define PHI(_a) normal_cumulative_distribution(_a)
    #define Z(_a) standard_normal_distribution(_a)

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


    /** greeks is a structure meant to store greek values involved in portfolio modeling. */
    typedef struct _greeks {

      
      /** stores delta part of the greeks */
      float delta;

      /** stores gamma part of the greeks */
      float gamma;

      /** stores theta part of the greeks */
      float theta;

      /** stores vega part of the greeks */
      float vega;

      /** stores rho part of the greeks */
      float rho;

    } _greeks;

    /** fibo_levels is a structure that stores the different fibonacci retracements for a time-serie */

    typedef struct fibo_levels {

      /** Stores level 0% */
      float p0;
      
      /** Stores level 23.6 % */
      float p23;
      
      /** Stores level 38.2 % */
      float p38;

      /** Stores level 50%. Please note that this is not really a fibo level 
       *  but since it is highly used, it is also computed. */
      float p50;

      /* Stores level 61.8 % */
      float p61;

      /* Stores level 100 % */
      float p100;

    } fibo_levels;


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
     * Detects if 2 value datasets cross each other.
     * @param recs1 first values dataset.
     * @param recs2 second values dataset.
     * @return true if 2 datasets cross each other, false otherwise.
     */
    bool crosses(std::vector<quotek::data::record>& recs1,
                             std::vector<quotek::data::record>& recs2);


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
     * This function returns a weight average of the values contained in
     * the provided recs dataset.
     * @param recs dataset to work on.
     * @param weights weights vector.
     * @return the mean value of the dataset, as a float value.
     */
    float weighted_average(std::vector<quotek::data::record>& recs,
                           std::vector<int>& weights);

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
     * a previous compute.
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
     * @sample if true, computes sample standard deviation, population standard deviation otherwise.
     * @return the standard deviation of the provided dataset, as a float.
     */
    float standard_deviation(std::vector<quotek::data::record>& recs, bool sample);

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
     * Computes the exponential moving average (EMA) of the provided dataset for n periods.
     * As a reminder, EMA is is a special kind of moving average giving more weight to recent values.
     * eg if you want to compute an EMA100 for the provided dataset, use exponential_moving_average(recs,50)
     * @param recs dataset to work on.
     * @param periods the number of dataset values that must be aggregated together to compute a single point of the EMA.
     * @return a vector of floats containing the graph values of the moving average for the dataset.
     */
    std::vector<float> exponential_moving_average(std::vector<quotek::data::record>& recs, 
                                                  int periods);


    /** Computes the weighted moving average (WMA) of the provided dataset for n periods.
     *  WMA gives even more weight to recent values than EMA.
     *  Note: the weight vector for this function is {1,2,..periods-1,periods}
     *  @param recs dataset to work on.
     *  @param periods Number of periods for the moving average.
     */

    std::vector<float> weighted_moving_average(std::vector<quotek::data::record>& recs, 
                                                  int periods);

    /**
     * linear_regression computes the linear regression of the provided dataset.
     * Note about linear regression: in order to avoid troublesome scaling problems,
     * it is computed which sequence(1..dataset_size) as x coordinates. So in order 
     * to work properly, it assumes constant interval on x between points of the dataset.
     * @param recs dataset to work on.
     * @param result reference, to store the result of the linear regression.
     * @return none.
     */
    void linear_regression(std::vector<quotek::data::record>& recs, std::vector<float>& result);
    
    /**
     * Computes the linear regression of the values contained in the dataset using an ordinary least
     * squares method, and gives the result as an affine structure such as the line equation is y = affine.a * x + affine.b
     * Note about linear regression: in order to avoid troublesome scaling problems,
     * it is computed which sequence(1..dataset_size) as x coordinates. So in order 
     * to work properly, it assumes constant interval on x between points of the dataset.
     * @param recs dataset to work on.
     * @return affine structure containing slope and intercept values.
     */
    affine linear_regression(std::vector<quotek::data::record>& recs);

    /**
     * This fucntion computes a polynomial regression from a provided recs dataset.
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

 
    /**
     * This function returns the cumulative normal density (PHI) for a given value.
     * @param value The value to compute PHI from.
     * @return CDF for value.
     */
    float normal_cumulative_distribution(float value);

    /**
     *  This function returns the standard normal distribution of a given value.
     *  @param value The value to compute SND from.
     *  @return Normal distribution for value.
     */
     float standard_normal_distribution(float value);

  }
}

#endif
