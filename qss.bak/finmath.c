#include "finmath.h"


int above_f(farray* values, float val,float thold) {

  int nb_above = 0;
  int i;
  float computed_thold;


  for(i=0;i<values->size;i++) {
    if (val > values->values[i]) nb_above++;
  }

  computed_thold = (float) (nb_above / values->size ); 

  if ( computed_thold >= thold ) return 1;

  return 0;

}


int below_f(farray* values, float val,float thold) {

  int nb_below = 0;
  int i;
  float computed_thold;


  for(i=0;i<values->size;i++) {
    if (val < values->values[i]) nb_below++;
  }

  computed_thold = (float) (nb_below / values->size ); 

  if ( computed_thold >= thold ) return 1;

  return 0;

}



float min_f(farray* values) {

  float min = 300000000;
  float cur = 0;
  int i;

  for(i=0;i<values->size;i++) {
     cur = values->values[i];
     if ( cur < min ) {
       min = cur;
     }
  }
  return min;
}


float max_f(farray* values) {

  float max = -300000000;
  float cur = 0;
  int i;

  for(i=0;i<values->size;i++) {
     cur = values->values[i];
     if ( cur > max ) {
       max = cur;
     }
  }
  return max;
}


float avg_f(farray* values) {
  float sum = 0;
  int i;

  for(i=0;i<values->size;i++) {
    sum += values->values[i];
  }
  return sum / values->size;
}


float cavg_f(float point,float average,int count) {
  return (float) (average + ((point - average) / count));
}



float variance_f(farray* values, uint8_t sample) {
  float mean = avg(values);
  float variance = 0;
  int i;
  for(i=0;i<values->size;i++) {
    variance += pow( values->values[i] - mean,2);    
  }

  if (sample) {
    variance /= ( values->size -1 ); 
  }
  
  else {
    variance /= values->size;
  }
  return variance;
}

float variance_q_f(farray* values, uint8_t sample,float avg1) {
  float mean = avg1;
  float variance = 0;
  int i;
  for(i=0;i<values->size;i++) {
    variance += pow( values->values[i] - mean,2);    
  }

  if (sample) {
    variance /= ( values->size -1 ); 
  }
  
  else {
    variance /= values->size;
  }
  return variance;
}




float covariance_f(farray* v1,farray* v2) {

  float v1_a = avg(v1);
  float v2_a = avg(v2);

  float covariance = 0;
  int i;
  for(i=0;i<v1->size;i++) {
    covariance += (v1->values[i] - v1_a) * (v2->values[i] - v2_a );    
  }

  covariance /= v1->size;
 return covariance;
}

float covariance_q_f(farray* v1,farray* v2,float avg1,float avg2) {

  float covariance = 0;
  int i;
  for(i=0;i<v1->size;i++) {
    covariance += (v1->values[i] - avg1) * (v2->values[i] - avg2 );    
  }

  covariance /= v1->size;
 return covariance;
}


farray* lreg_f(farray* v2) {

  farray v1;
  farray* results = (farray*) malloc(sizeof(farray));
  float a,b;
  float avg1,avg2;
  int i;

  farray_init(results,10);
  farray_init(&v1,10);

  //initializing of X array of values
  for(i=1;i<=v2->size;i++) {
    farray_push(&v1,(float)i);
  }

  avg1 = avg(&v1);
  avg2 = avg(v2);

  a = covariance_q(&v1,v2,avg1,avg2) / variance_q(&v1,0,avg1);
  b = avg2 - ( a * avg1 ) ; 

  for(i=0;i<v2->size;i++) {
    float r = (float) a* v1.values[i] + b;
    farray_push(results,r);
  } 

  farray_destroy(&v1);
  return results;

}


farray* lreg_coefs_f(farray* v2) {

  farray v1;
  farray* results = (farray*) malloc(sizeof(farray));
  float a,b;
  float avg1,avg2;
  int i;

  farray_init(results,2);
  farray_init(&v1,10);

  //initializing of X array of values
  for(i=1;i<=v2->size;i++) {
    farray_push(&v1,(float)i);
  }

  avg1 = avg(&v1);
  avg2 = avg(v2);

  a = covariance_q(&v1,v2,avg1,avg2) / variance_q(&v1,0,avg1);
  b = avg2 - ( a * avg1 ) ; 

  farray_push(results,a);
  farray_push(results,b);  

  farray_destroy(&v1);
  return results;

}


float stdDeviation_f(farray* values) {
  return (float) sqrt(variance(values,0));
}


farray* cmavg_f(farray* values) {

  int i = 0;
  float lastav = 0;
  farray* results = (farray*) malloc(sizeof(farray));
  farray_init(results,10);

  for (i = 0; i<values->size;i++)
  {
    lastav = cavg(values->values[i], lastav,i+1);
    farray_push(results,lastav);
  }
  return results;
}


float percentDelta_f(farray* values) {
  float v0 = values->values[0];
  float v1 = farray_last(values);
  return ( (v1 - v0) / v0 ) * 100 ;
}