#include "queue_c.h"
#include <stdlib.h>

#define MinQueueSize (5)

struct QueueRecord {
  int Capacity;
  int Front;
  int Rear;
  int Size;
  ElementType *Array;
};

int IsEmpty(Queue_c Q) {
  return Q->Size == 0;
}

int IsFull(Queue_c Q) {
  if ( Q->Size == Q->Capacity) return 1;
  else return 0;
}

Queue_c CreateQueue(int MaxElements) {
  Queue_c Q;

  if (MaxElements < MinQueueSize) {
    Error("CreateQueue Error: Queue_c size is too small.");
  }

  Q = (Queue_c) malloc (sizeof(struct QueueRecord));
  if (Q == NULL) {
    FatalError("CreateQueue Error: Unable to allocate more memory.");
  }

  Q->Array = (ElementType*) malloc( sizeof(ElementType) * MaxElements );
  if (Q->Array == NULL) {
    FatalError("CreateQueue Error: Unable to allocate more memory.");
  }

  Q->Capacity = MaxElements;
  MakeEmpty(Q);

  return Q;
}

void MakeEmpty(Queue_c Q) {

  Q->Size = 0;
  Q->Front = 1;
  Q->Rear = 0;

}

void DisposeQueue(Queue_c Q) {
  if (Q != NULL) {
    free(Q->Array);
    free(Q);
  }
}

static int Succ(int Value, Queue_c Q) {
  if (++Value == Q->Capacity) {
    Value = 0;
  }
  return Value;
}

void Enqueue(ElementType X, Queue_c Q) {

  if (IsFull(Q)) {
    Error("Enqueue Error: The queue is full.");
  } else {
    Q->Size++;
    Q->Rear = Succ(Q->Rear, Q);
    Q->Array[Q->Rear] = X;
  }

}

ElementType Front(Queue_c Q) {

  if (!IsEmpty(Q)) {
    return Q->Array[Q->Front];
  }
  Error("Front Error: The queue is empty.");

  /* Return value to avoid warnings from the compiler */
  return 0;

}

void Dequeue(Queue_c Q) {

  if (IsEmpty(Q)) {
    Error("Dequeue Error: The queue is empty.");
  } else {
    Q->Size--;
    Q->Front = Succ(Q->Front, Q);
  }

}

ElementType FrontAndDequeue(Queue_c Q) {

  ElementType X = 0;

  if (IsEmpty(Q)) {
    Error("FrontAndDequeue Error: The queue is empty.");
  } else {
    Q->Size--;
    X = Q->Array[Q->Front];
    Q->Front = Succ(Q->Front, Q);
  }
  return X;

}

