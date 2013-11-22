#ifndef MODULES_H
#define MODULES_H

#include "queue_c.h"
#include <string.h>

typedef struct eval_module_io {
      char mname[50];
      Queue_c* input;
      Queue_c* output;
} eval_module_io;

struct evmio_array {
  eval_module_io* evmio;
  size_t size;
};


eval_module_io* resolveIO(struct evmio_array* evmio_a, char* name);
int __module_input(struct evmio_array* evmio_a, char* name,char* input);

#define module_input(_name,_input) (  __module_input(evmio_a,_name,_input) )


#endif




