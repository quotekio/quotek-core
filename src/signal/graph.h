#ifndef GRAPH_H
#define GRAPH_H

#include <gtk/gtk.h>
#include <goocanvas-2.0/goocanvas.h>
#include "../lib/narrays.h"
#include "../lib/utils.h"

#include "dump.h"
#include <string>


typedef struct coords {

  int x;
  int y;

} coords;


class graph {

  public:
    graph(dump*,string,int);
    void draw();
    void setCanvas(GtkWidget* c);
    coords translateCoordinates(int,float);
    string getPath();
  private:

    void findMin();
    void findMax();

    string name;
    iarray* tstamps;
    farray* values; 
    int signal_interval;
    GtkWidget* canvas;
    GooCanvasItem* root;
  
    GooCanvasItem** graphlines;

    float vmin;
    float vmax;
    float delta;
    int width;
    int height;
    int pixels_per_point;
    int pixels_per_second;
    int zoom_x;

};

#endif