#define POS_OPEN 0x00
#define POS_PENDING_CLOSE 0x01
#define POS_CLOSED 0x02

typedef struct position {
  const char* indice;
  const char* epic;
  const char* dealid;
  const char* name;
  int size;
  float stop;
  float vstop;
  int nb_inc;
  float limit;
  float open;
  float pnl;
  int status;
} position;

