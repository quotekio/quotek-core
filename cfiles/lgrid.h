#ifndef LGRID_H
#define LGRID_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define LCELL_NAND 0x01
#define LCELL_NOR 0x02
#define LCELL_NXOR 0x03
#define LCELL_AND 0x04
#define LCELL_OR 0x05
#define LCELL_XOR 0x06


typedef struct lcell {

  uint8_t *input_1;
  uint8_t *input_2;
  uint8_t   output;
  uint8_t gate_type;

} lcell;


typedef struct lconn {

  int c1_x;
  int c1_y;
  int c2_x;
  int c2_y;
  uint8_t in_num;

} lconn;


typedef struct lgrid {

  int gridsize_x;
  int gridsize_y;
  int nb_inputs;
  int nb_outputs;
  lcell** grid;
  lconn* connections;
  int nb_conn;
 
} lgrid;


void lcell_connect(lcell*,lcell*,int);
void lgrid_connect(lgrid*,int,int,int,int,int);

void lgrid_init(lgrid*,int,int,int,int);
char* lgrid_serialize(lgrid*);
lgrid* lgrid_load(const char*);
int lgrid_dump(lgrid* lg,const char*);

lgrid* lgrid_cross(lgrid*,lgrid*);
lgrid* lgrid_mutate(lgrid*,float);

void lgrid_input(lgrid*,uint8_t*);
void lgrid_process(lgrid*);
uint8_t* lgrid_get_outputs(lgrid* lg);

#endif




