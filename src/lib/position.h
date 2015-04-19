#ifndef POSITION_H
#define POSITION_H
typedef struct position {
  std::string indice;
  std::string epic;
  std::string dealid;
  std::string name;
  int size;
  float stop;
  float vstop;
  int nb_inc;
  float limit;
  float vlimit;
  float open;
  float pnl;
  float pnl_peak;
  int status;
  int open_time;
  int close_time;
} position;

#endif