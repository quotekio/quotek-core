#include "narrays.h"

void* farray_init(farray* f,int size) {
  f->values = (float*) malloc( size * sizeof(float) );
  f->size = 0;
  f->msize = size;
}

void* farray_destroy(farray* f) {
  free(f->values);
}


void* iarray_init(iarray* i,int size) {
  i->values = (uint32_t*) malloc( size * sizeof(uint32_t) );
  i->size = 0;
  i->msize = size;
}


void* farray_push(farray* f,float value) {

  if (!f->values) {
    farray_init(f,100);
  }

  int pos = f->size;
  if (pos == f->msize - 1) {
    f->values = (float*) realloc(f->values, sizeof(float) * (f->msize+100));
    f->msize+=100;
  }

  f->values[pos] = value;
  f->size++;
}

void* iarray_push(iarray* i,uint32_t value) {

  if (!i->values) {
    iarray_init(i,100);
  }

  int pos = i->size;
  if (pos == i->msize - 1) {
    i->values = (uint32_t*) realloc(i->values, sizeof(uint32_t) * (i->msize+100));
    i->msize+=100;
  }

  i->values[pos] = value;
  i->size++;
}

float farray_last(farray* f) {
  if (f->size > 0) return f->values[f->size-1];
  else return 0;
}

uint32_t iarray_last(iarray* i) {
  if (i->size > 0) return i->values[i->size-1];
  else return 0;
}

int iarray_dicho_search(iarray* i,uint32_t s) {

   int first = 0;
   int last = i->size -1;
   int middle = (first+last)/2;
 
   while( first <= last )
   {
      if ( i->values[middle] < s )
         first = middle + 1;    
      else if ( i->values[middle] == s ) 
      {
        return middle ;
      }
      else last = middle - 1;
      middle = (first + last)/2;
   }

   return -1;

}

