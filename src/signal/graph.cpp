#include "graph.h"

graph::graph(dump* d1,string n,int si) {
  name = n;
  tstamps = d1->getTStamps();
  values = &(d1->getValues()->get(n));
  signal_interval = si;
  canvas = NULL;
  root = NULL;

  findMin();
  findMax();
  delta = vmax - vmin;
  graphlines = (GooCanvasItem**) malloc ( sizeof(GooCanvasItem*) * values->size ) ;

  width = 4000;
  height = 400;

  pixels_per_point = height / delta; 

  zoom_x = 60;

}


coords graph::translateCoordinates(int t,float value) {
  
  int elapsed = t - tstamps->values[36000];
  coords result;
  result.x = elapsed * pixels_per_second;
  result.y = (vmax - value) * pixels_per_point; 
  return result;
}

string graph::getPath() {

  string result = "";  
  coords c0 = translateCoordinates(tstamps->values[36000], values->values[36000] );

  result = "M" + int2string(c0.x) + " " + int2string(c0.y) ;

  //for(int i=1;i<values->size;i++ ) {
    for (int i=36000;i<40000;i++) {
    coords c1 = translateCoordinates(tstamps->values[i], values->values[i] );
    result = result + " L" + int2string(c1.x) + " " + int2string(c1.y);
  }

  return result;

}

void graph::draw() {
  /*for(int i=0;i<values->size-1;i++ ) {
    coords c1 = translateCoordinates(tstamps->values[i], values->values[i] );
    coords c2 = translateCoordinates(tstamps->values[i+1], values->values[i+1] );
    //graphlines[i] = goo_canvas_

  }*/
 
  g_print("* DRAW *\n");

  string path = getPath();

  GooCanvasItem *cpath = goo_canvas_path_new (root,
                                              path.c_str(),
                                              "stroke-color", "red",
                                              NULL);

}

void graph::setCanvas(GtkWidget* c) {
  canvas = c;
  root = goo_canvas_get_root_item (GOO_CANVAS(canvas));
}


/* PRIVATE METHODS */
void graph::findMin() {
  vmin = 0xffffffff;
  for (int i=0;i<values->size;i++) {
    if ( values->values[i] < vmin ) {
      vmin = values->values[i];
    }
  }
}

void graph::findMax() {
  vmax = 0x0000000;
  for (int i=0;i<values->size;i++) {
    if ( values->values[i] > vmax ) {
      vmax = values->values[i];
    }
  }
}