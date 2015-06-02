#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define Error(Str)        FatalError(Str)
#define FatalError(Str)   fprintf(stderr, "%s\n", Str)

typedef char* ElementType;

#ifndef _Queue_h

  struct QueueRecord;
  typedef struct QueueRecord *Queue_c;

  int         IsEmpty(Queue_c Q);
  int         IsFull(Queue_c Q);
  Queue_c       CreateQueue(int MaxElements);
  void        DisposeQueue(Queue_c Q);
  void        MakeEmpty(Queue_c Q);
  void        Enqueue(ElementType X, Queue_c Q);
  ElementType Front(Queue_c Q);
  void        Dequeue(Queue_c Q);
  ElementType FrontAndDequeue(Queue_c Q);

#endif  /* _Queue_h */

#endif

