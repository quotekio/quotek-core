#ifndef POSITION_H
#define POSITION_H
typedef struct position {
  string indice;
  string epic;
  string dealid;
  string name;
  int size;
  float stop;
  float vstop;
  int nb_inc;
  float limit;
  float vlimit;
  float open;
  float pnl;
  int status;
  int open_time;
  int close_time;
} position;

#endif