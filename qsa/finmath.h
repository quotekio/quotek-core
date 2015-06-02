#include "narrays.h"
#include <math.h>

int above_f(farray* values, float val,float thold);
int below_f(farray* values, float val,float thold);


float min_f(farray*);
float max_f(farray*);

float avg_f(farray*);
float cavg_f(float,float,int);

float variance_f(farray*,uint8_t);
float variance_q_f(farray*, uint8_t,float);

float covariance_f(farray*,farray*);
float covariance_q_f(farray*,farray*,float,float);

float stdDeviation_f(farray*);
float percentDelta_f(farray*);

farray* cmavg_f(farray*);
farray* lreg_f(farray*);
farray* lreg_coefs_f(farray*);


