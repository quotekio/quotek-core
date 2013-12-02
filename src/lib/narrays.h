#ifndef NARRAYS_H
#define NARRAYS_H
#include <stdlib.h>
#include <stdint.h>

typedef struct farray {

  float* values;
  int size;
  int msize;

} farray;

typedef struct iarray {

  uint32_t* values;
  int size;
  int msize;

} iarray;

void farray_init(farray* f,int size);
void iarray_init(iarray* i,int size);
void farray_push(farray* f,float value);
void farray_destroy(farray* f);

void iarray_push(iarray* i,uint32_t value);
float farray_last(farray* f);
uint32_t iarray_last(iarray* i);


#endif