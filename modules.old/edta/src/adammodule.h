#include "assoc.h"
#include "narrays.h"
#include "queue_c.h"
#include "store.h"

typedef struct module_io {
  float* cur_pnl;
  float* cumulative_pnl;
  Queue_c* input;
  Queue_c* output;
  store* s;
} module_io;

typedef struct eval_module_io {
  char mname[50];
  Queue_c* input;
  Queue_c* output;
} eval_module_io;
