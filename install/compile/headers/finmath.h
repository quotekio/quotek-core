#include "narrays.h"
#include <math.h>

int above(farray* values, float val,float thold);
int below(farray* values, float val,float thold);


float min(farray*);
float max(farray*);

float avg(farray*);
float cavg(float,float,int);



float variance(farray*,uint8_t);
float variance_q(farray*, uint8_t,float);

float covariance(farray*,farray*);
float covariance_q(farray*,farray*,float,float);

float stdDeviation(farray*);
float percentDelta(farray*);


farray* cmavg(farray*);
farray* lreg(farray*);
farray* lreg_coefs(farray*);


