#include <math.h>
#include "narrays.h"
#include "records.h"


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




float stdDeviation(records*);
float percentDelta(records*);


farray* cmavg(records*);
farray* lreg(records*);
farray* lreg_coefs(records*);


