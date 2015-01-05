#ifndef STORE_H
#define STORE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STORENAME_LEN 256
#define ERR 0xffffffff

typedef struct store {

  char name[128][MAX_STORENAME_LEN];
  void* data[128];
  int size;
  int msize;
  pthread_mutex_t m_mutex;
  pthread_cond_t  m_condition;  

} store;

void* store_push(store* s, char* name, void* data);
void* store_get(store* s, char* name);
void store_clean(store* s, char* name);
void* store_init(store* s);
int store_exists(store* s,char* name);

#endif
