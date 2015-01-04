#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STORENAME_LEN 256
#define ERR 0xffffffff

typedef struct store {

  char** name;
  void* data[128];
  int size;
  int msize;
  
} store;


void* store_push(store* s, char* name, void* data);
void* store_get(store* s, char* name);
void store_clean(store* s, char* name);
void* store_init(store* s,int size);
int store_exists(store* s,char* name);
