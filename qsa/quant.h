/*
Quotek Strats API 1.1
Copyright 2013-2015 Quotek SAS
*/

#include <math.h>
#include "narrays.h"
#include "records.h"

/** This struct aims to store affine equations parameters. */
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
 * This functions helps to rescale records to get correct value/timesteamp
 * ratio
 */
records* scale_records(records* recs1, int scale_x, int scale_y);

/**
 * This function computes the trend of an asset on a specific time period and returns
 * a triplet of values: the bull, bear and neutral percentages.
 */
trend_p trend_percentages(records* recs);

int above(records* recs, float val,float thold);
int below(records* recs, float val,float thold);


float min(records*);
float max(records*);

float avg(records*);
float cavg(float,float,int);

float variance(records*,uint8_t);
float variance_q(records*, uint8_t,float);

float covariance(records*,records*);
float covariance_q(records*,records*,float,float);

//Black-Scholes function, gives back option price.
float euro_bsf(const char* way, float stock_price, float strike, float expire, float rf_interest, float Volatility );

//Black-Scholes reverse function, gives back Implied Volatility.
float reverse_euro_bsf(const char* way, float option_price, float stock_price, float strike, float expire, float rf_interest );

//time value of money
float tvom(float interest, float t);


float stdDeviation(records*);
float percentDelta(records*);

farray* cmavg(records*);
farray* lreg(records*);



/**
 * This function produces the affine equation of the linear regression
 * of the points inside recs2
 */
affine lreg_affine(records* recs2);
affine lreg_affine_scaled(records* recs2, int scale_x, int scale_y);

/**
 * Computes risk reward ratio given expected_gain, open price, stop, and pip cost.
 *
 */
float risk_reward_ratio(float expected_profit,
	                    int stop, 
	                    int leverage);
/**
 * Computes where to place the stop-loss given expected risk reward ratio.
 */
float compute_stop_from_risk_reward_ratio(float risk_reward_ratio, 
	                                      float expected_profit, 
	                                      int leverage);





