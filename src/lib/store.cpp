#include "store.h"

void store_push(store* s,char* name,uint32_t data)  {

  pthread_mutex_lock( &s->m_mutex );
  int exists = store_exists(s,name);

  if (exists == ERR) {
    s->data[s->size] = data;
    strncpy(s->name[s->size],name,MAX_STORENAME_LEN * sizeof(char));
    s->size++;
  }

  else {
    s->data[exists] = data;
  }

  pthread_mutex_unlock( &s->m_mutex );

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


uint32_t store_get(store* s,char* name) {

  int i;
  pthread_mutex_lock( &s->m_mutex );
  for (i=0;i<s->size;i++) {
    if (strcmp(s->name[i],name) == 0 ) {
      return s->data[i];
    }
  }
  return ERR;
}


void store_init(store* s,int size) {

  int i = 0;
  //s->data = (void**) malloc(size * sizeof(void*)) ;
  s->name = (char**) malloc(size * sizeof(char*));

  for (i=0;i< size;i++) {
    s->name[i] = (char*) malloc( MAX_STORENAME_LEN * sizeof(char) );
    //s->data[i] = (void*) malloc( 1 * sizeof(char) );
  }

  s->size = 0;
  s->msize = size;

  pthread_mutex_init( &s->m_mutex, NULL );
  pthread_cond_init( &s->m_condition, NULL );

}

char* store_item_at(store* s,int index) {
  if (index < s->size ) return s->name[index];
  else return NULL;
}


uint32_t store_value_at(store*s,int index) {

 if (index < s->size ) return s->data[index];
 else return 0; 

}

void store_clear(store* s) {
  int i=0;
  for (i=0;i<s->size;i++) {
    strcpy(s->name[i],"\0");
    s->data[i] = 0x00;
  }
}