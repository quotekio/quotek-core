#include "lgrid.h"

void lcell_connect(lcell* n1,lcell* n2,int input_num) {

  if (input_num == 1) {
    n2->input_1 = &(n1->output);
  }

  else {
    n2->input_2 = &(n1->output);
  }

}

void lgrid_connect(lgrid* lg,int cell1_x,int cell1_y,int cell2_x,int cell2_y,int input_num) {

  lcell* c1 = &(lg->grid[cell1_x][cell1_y]);
  lcell* c2 = &(lg->grid[cell2_x][cell2_y]);
  lcell_connect(c1,c2,input_num);

  lg->connections[lg->nb_conn].c1_x = cell1_x;
  lg->connections[lg->nb_conn].c1_y = cell1_y;
  lg->connections[lg->nb_conn].c2_x = cell2_x;
  lg->connections[lg->nb_conn].c2_y = cell2_y;

  lg->connections[lg->nb_conn].in_num = input_num;

  lg->nb_conn++;

}


void lgrid_init(lgrid* lg,int gridsize_x,int gridsize_y,int nb_inputs,int nb_outputs) {

  int i,j;

  if (nb_inputs % 2 != 0) {
    fprintf(stderr,"*CRITICAL ERROR: invalid number of inputs (must be even)*\n");
    return;
  }
  
  lg->connections = (lconn*) malloc( 2 * (gridsize_x - 1 ) * gridsize_y * sizeof(lconn) );
  lg->nb_conn = 0; 

  lg->gridsize_x = gridsize_x;
  lg->gridsize_y = gridsize_y;
  lg->nb_inputs = nb_inputs;
  lg->nb_outputs = nb_outputs;

  lg->grid = (lcell**) malloc( gridsize_x * sizeof(lcell*) );

  for (i=0;i<gridsize_x;i++) {
    lg->grid[i] = (lcell*) malloc( gridsize_y * sizeof(lcell) );
  }

  //initialize all cells to NAND, and puts input pointers to NULL
  for(i=0;i<gridsize_x;i++) {
    for(j=0;j<gridsize_y;j++) {
      lg->grid[i][j].gate_type = LCELL_NAND;
      lg->grid[i][j].input_1 = NULL;
      lg->grid[i][j].input_2 = NULL;
    }
  }

}

int lgrid_dump(lgrid* lg,const char* filename) {

  char buff[4096];
  FILE* dfh;
  int i,j;

  dfh = fopen(filename,"w");

  if (!dfh ) return 1;

  sprintf(buff,"GRID %d %d %d %d\n", lg->gridsize_x,lg->gridsize_y,lg->nb_inputs,lg->nb_outputs);
  fputs(buff,dfh);

  for(i=0;i<lg->gridsize_x;i++) {
    for(j=0;j<lg->gridsize_y;j++) {
      sprintf(buff,"CELL %d %d %d\n",i,j,lg->grid[i][j].gate_type);
      fputs(buff,dfh);
    }
  }

  for(i=0;i<lg->nb_conn;i++) {
    sprintf(buff,"CONN %d %d %d %d %d\n",
    	    lg->connections[i].c1_x,
    	    lg->connections[i].c1_y,
    	    lg->connections[i].c2_x,
    	    lg->connections[i].c2_y,
    	    lg->connections[i].in_num);
    fputs(buff,dfh);
  }

  fclose(dfh);

}




lgrid* lgrid_load(const char* filename) {

  FILE* dfh;
  int i;
  int x,y;
  int gsize_x,gsize_y,nb_in,nb_out;
  int c1_x,c1_y,c2_x,c2_y,gt,in_num;
  char buff[4096];

  lgrid* lg = (lgrid*) malloc(sizeof(lgrid));
  dfh = fopen(filename,"r");

  while (!feof(dfh) ) {
    fgets(buff,4096,dfh);

    if (strstr(buff,"GRID ") == buff ) {
      sscanf(buff,"GRID %d %d %d %d",&gsize_x,&gsize_y,&nb_in,&nb_out);
      lgrid_init(lg,gsize_x,gsize_y,nb_in,nb_out);
    }

    else if ( strstr(buff,"CELL ") == buff ) {
      sscanf(buff,"CELL %d %d %d",&x,&y,&gt);
      lg->grid[x][y].gate_type = gt; 
    }

    else if ( strstr(buff,"CONN ") == buff ) {
      sscanf(buff,"CONN %d %d %d %d %d",&c1_x,&c1_y,&c2_x,&c2_y,&in_num);
      lgrid_connect(lg,c1_x,c1_y,c2_x,c2_y,in_num);
    }

    buff[0] = '\0';
  }

  fclose(dfh);
  return lg;
}


void lgrid_randomize(lgrid* lg) {

  int i,j;

  srand(time(NULL));
  int nb_connec = rand() % ( ( 2 * (lg->gridsize_x -1) * lg->gridsize_y ) + 1 )  ;
  if (nb_connec == 0) nb_connec++;

  //gate types
  for(i=0;i<lg->gridsize_x;i++) {
    for(j=0;j<lg->gridsize_y;j++) {
      int gt = rand() % 7;
      lg->grid[i][j].gate_type = gt;
    }
  }

  //connections
  for (i=0;i<nb_connec;i++) { 
    int cell1_x = rand() % (lg->gridsize_x) ;
    int cell1_y = rand() % (lg->gridsize_y);
    int cell2_x = rand() % (lg->gridsize_x) ;
    int cell2_y = rand() % (lg->gridsize_y);        
    int in_num = rand() % (3-1) + 1;
    lgrid_connect(lg,cell1_x,cell1_y,cell2_x,cell2_y,in_num);
  }
}


lgrid* lgrid_mutate(lgrid* lg,float mr) {

}

lgrid* lgrid_cross(lgrid* lg1,lgrid* lg2) {

  lgrid* res = (lgrid*) malloc(2 * sizeof(lgrid));

  //cannot mate grids of != sizes and i/o
  if (  lg1->gridsize_x != lg2->gridsize_x || 
        lg1->gridsize_y != lg2->gridsize_y ||
        lg1->nb_inputs != lg2->nb_inputs   ||
        lg1->nb_outputs != lg2->nb_outputs
  ) return NULL;

  return res;

}

void lgrid_input(lgrid* lg,uint8_t* inputs) {

  int i,offs;

  offs = 0;
  for (i=0;i<lg->nb_inputs/2;i++) {
 
    lcell* c = &(lg->grid[0][i]);
  
    c->input_1 = &(inputs[i+offs]);
    c->input_2 = &(inputs[i+offs+1]);
 
    if (*(c->input_1) == 1) *(c->input_1) = 0xff;
    if (*(c->input_2) == 1) *(c->input_2) = 0xff;

    offs++;

  }

}

void lgrid_process(lgrid* lg) {

  int i,j;

  for (i=0;i<lg->gridsize_x;i++) {
    for (j=0;j<lg->gridsize_y;j++) {
      lcell* c = &(lg->grid[i][j]);
      c->output = 0;
      if ( c->input_1 != NULL && c->input_2 != NULL ) {

        printf("PROCESSING CELL [%d,%d]\n",i,j);

        switch(c->gate_type) {

          case LCELL_NAND:
            printf("APPLYING NAND(in_1:%d,in_2:%d)\n",*(c->input_1),*(c->input_2) );
            c->output = ~ (*(c->input_1) & *(c->input_2) );
            break;
          case LCELL_NOR:
            c->output = ~ ( *(c->input_1) | *(c->input_2) );
            break;
          case LCELL_NXOR:
            c->output = ~ ( *(c->input_1) ^ *(c->input_2) );
            break; 
          case LCELL_AND:
            printf("APPLYING AND(in_1:%d,in_2:%d)\n",*(c->input_1),*(c->input_2) );
            c->output = *(c->input_1) & *(c->input_2) ;
            break;
          case LCELL_OR:
            c->output = *(c->input_1) | *(c->input_2) ;
            break;
          case LCELL_XOR:
            c->output = *(c->input_1) ^ *(c->input_2) ;
            break;
        }
         printf("CELL [%d,%d] OUTPUT: %d\n",i,j,c->output);
      }
    }
  }
}


uint8_t* lgrid_get_outputs(lgrid* lg){

  int i;
  uint8_t* result = (uint8_t*) malloc( lg->nb_outputs * sizeof(uint8_t) );

  lcell* output_cells_line = lg->grid[lg->gridsize_x-1]; 

  for (i=0;i<lg->nb_outputs;i++) {

    if (output_cells_line[i].output == 0xff ) result[i] = 1;
    else result[i] = 0;
  }

  return result;
  
}

