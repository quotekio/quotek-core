#include "store.h"

void* store_push(store* s,char* name, void* data)  {

  int exists = store_exists(s,name);

  if (exists == ERR) {
    s->data[s->size] = data;
    strncpy(s->name[s->size],name,MAX_STORENAME_LEN * sizeof(char));
    s->size++;
  }

  else {
    s->data[exists] = data;
  }
}


int store_exists(store* s,char* name) {

  int i;
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
      return i;
    } 
  }
  return ERR;
}


void* store_get(store* s,char* name) {

  int i;
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
      return s->data[i];
    }
  }
  return NULL;
}


void store_clean(store* s, char* name) {

  int i;
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
  
      store_shift_left(s,i);

      int j = 0;
      for (j=0;j< MAX_STORENAME_LEN; j++) s->name[s->size-1][j] = 0x00;
      s->data[s->size-1] = 0;
      s->size = s->size -1;

    }
  }

}

void store_shift_left(store* s, int offset) {
  int i=0;
  for (i= offset + 1; i < s->size ; i++ ) {
    s->data[i-1] = s->data[i];
    strcpy(s->name[i-1], s->name[i]);
  }
}

void* store_init(store* s) {

  s->size = 0;
  s->msize = 128;

}

