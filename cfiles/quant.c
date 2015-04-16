#include "quant.h"

int above(records* recs, float val,float thold) {

  int nb_above = 0;
  int i;
  float computed_thold;

  for(i=0;i<recs->size;i++) {
    if (val > recs->data[i].value) nb_above++;
  }

  computed_thold = (float) (nb_above / recs->size ); 
  if ( computed_thold >= thold ) return 1;
  return 0;
}

int below(records* recs, float val,float thold) {

  int nb_below = 0;
  int i;
  float computed_thold;

  for(i=0;i<recs->size;i++) {
    if (val < recs->data[i].value) nb_below++;
  }

  computed_thold = (float) (nb_below / recs->size ); 
  if ( computed_thold >= thold ) return 1;
  return 0;

}


float min(records* recs) {

  float min = 300000000;
  float cur = 0;
  int i;

  for(i=0;i<recs->size;i++) {
     cur = recs->data[i].value;
     if ( cur < min ) {
       min = cur;
     }
  }
  return min;
}


float max(records* recs) {

  float max = -300000000;
  float cur = 0;
  int i;

  for(i=0;i<recs->size;i++) {
     cur = recs->data[i].value;
     if ( cur > max ) {
       max = cur;
     }
  }
  return max;
}



float avg(records* recs) {
  float sum = 0;
  int i;

  for(i=0;i<recs->size;i++) {
    sum += recs->data[i].value;
  }
  return sum / recs->size;
}


float cavg(float point,float average,int count) {
  return (float) (average + ((point - average) / count));
}

float variance(records* recs, uint8_t sample) {
  float mean = avg(recs);
  float variance = 0;
  int i;
  for(i=0;i<recs->size;i++) {
    variance += pow( recs->data[i].value - mean,2);    
  }

  if (sample) {
    variance /= ( recs->size -1 ); 
  }
  
  else {
    variance /= recs->size;
  }
  return variance;
}

float variance_q(records* recs, uint8_t sample,float avg1) {
  float mean = avg1;
  float variance = 0;
  int i;
  for(i=0;i<recs->size;i++) {
    variance += pow( recs->data[i].value - mean,2);    
  }

  if (sample) {
    variance /= ( recs->size -1 ); 
  }
  
  else {
    variance /= recs->size;
  }
  return variance;
}




float covariance(records* recs1,records* recs2) {

  float v1_a = avg(recs1);
  float v2_a = avg(recs2);

  float covariance = 0;
  int i;
  for(i=0;i<recs1->size;i++) {
    covariance += (recs1->data[i].value - v1_a) * (recs2->data[i].value - v2_a );    
  }

  covariance /= recs1->size;
 return covariance;
}


/**
 * Computes covariance without 
 */

float covariance_q(records* recs1,records* recs2,float avg1,float avg2) {

  float covariance = 0;
  int i;
  for(i=0;i<recs1->size;i++) {
    covariance += (recs1->data[i].value - avg1) * (recs2->data[i].value - avg2 );    
  }

  covariance /= recs1->size;
 return covariance;
}


farray* lreg(records* recs) {

  records recs1;
  farray* results = (farray*) malloc(sizeof(farray));
  float a,b;
  float avg1,avg2;
  int i;

  farray_init(results,10);
  records_init(&recs1,10);

  //initializing of X array of values
  for(i=1;i<=recs->size;i++) {
    record r;
    r.value = i;
    records_push(&recs1,r);
  }

  avg1 = avg(&recs1);
  avg2 = avg(recs);

  a = covariance_q(&recs1,recs,avg1,avg2) / variance_q(&recs1,0,avg1);
  b = avg2 - ( a * avg1 ) ; 

  for(i=0;i<recs->size;i++) {
    float r = (float) a* recs1.data[i].value + b;
    farray_push(results,r);
  } 

  //farray_destroy(&v1);
  return results;

}


affine lreg_affine(records* recs2) {

  affine result;

  records recs1;
  float a,b;
  float avg1,avg2;
  int i;

  records_init(&recs1,10);

  //initializing of X array of values
  for(i=1;i<=recs2->size;i++) {
    record r;
    r.value = (float) i;
    records_push(&recs1,r);
  }

  avg1 = avg(&recs1);
  avg2 = avg(recs2);

  result.a = covariance_q(&recs1, recs2, avg1, avg2) / variance_q(&recs1, 0, avg1);
  result.b = avg2 - ( a * avg1 ) ; 

  free(recs1.data);
  return results;

}

float stdDeviation(records* recs) {
  return (float) sqrt(variance(recs,0));
}

//time value of money
float tvom(float interest, float t) {
  return exp( interest * t);
}

farray* cmavg(records* recs) {

  int i = 0;
  float lastav = 0;
  farray* results = (farray*) malloc(sizeof(farray));
  farray_init(results,10);

  for (i = 0; i<recs->size;i++)
  {
    lastav = cavg(recs->data[i].value, lastav,i+1);
    farray_push(results,lastav);
  }
  return results;
}


float percentDelta(records* recs) {
  float v0 = recs->data[0].value;
  float v1 = (records_last(recs))->value;
  return ( (v1 - v0) / v0 ) * 100 ;
}

int spread_above(records* recs, float val,float thold) {

  int nb_above = 0;
  int i;
  float computed_thold;

  for(i=0;i<recs->size;i++) {
    if (val > recs->data[i].spread) nb_above++;
  }

  computed_thold = (float) (nb_above / recs->size ); 
  if ( computed_thold >= thold ) return 1;
  return 0;
}

int spread_below(records* recs, float val,float thold) {

  int nb_below = 0;
  int i;
  float computed_thold;

  for(i=0;i<recs->size;i++) {
    if (val < recs->data[i].spread) nb_below++;
  }

  computed_thold = (float) (nb_below / recs->size ); 
  if ( computed_thold >= thold ) return 1;
  return 0;

}


float spread_min(records* recs) {

  float min = 300000000;
  float cur = 0;
  int i;

  for(i=0;i<recs->size;i++) {
     cur = recs->data[i].spread;
     if ( cur < min ) {
       min = cur;
     }
  }
  return min;
}


float spread_max(records* recs) {

  float max = -300000000;
  float cur = 0;
  int i;

  for(i=0;i<recs->size;i++) {
     cur = recs->data[i].spread;
     if ( cur > max ) {
       max = cur;
     }
  }
  return max;
}

float spread_avg(records* recs) {
  float sum = 0;
  int i;

  for(i=0;i<recs->size;i++) {
    sum += recs->data[i].spread;
  }
  return sum / recs->size;
}


