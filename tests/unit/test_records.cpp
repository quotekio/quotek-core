#include "../../src/lib/records.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

using namespace std;

int main(int argc, char** argv) {
 
  records recs;
  record r; 
  records_init(&recs,100);

  int i =0;

  uint32_t t = time(0);

  for (i=0;i<10;i++)  {
    r.value = i;
    r.spread = i;
    r.timestamp = t + i;
    records_push(&recs,r);
    sleep(1);
    
  }

  assert(recs.size == 10);

  t = time(0);
  records* recs2 = records_sample(&recs,"dicho",t,-10,-5);
  assert(recs2->size == 5);
  record* r2 = records_last(recs2);
  assert( r2->value ==  4);

  cout << "TEST_RECORDS [OK]" << endl;

  return 0;

}