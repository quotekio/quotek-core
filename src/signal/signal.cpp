#include "signal.h"

preload_io pre_io;
graph* g;
GtkWidget* scanvas;

//#### PRELOAD WINDOW ######

preloadWindow::preloadWindow() {

  builder = gtk_builder_new();
  filename =  g_build_filename (GLADE_PRELOAD_WIN, NULL); 

  gtk_builder_add_from_file (builder, filename, &error); 
  g_free (filename); 
  if (error) 
  {
    gint code = error->code; 
    printf("ERROR!\n");
    g_error_free (error); 
  }  

  getWidgets();
  showAll();

}

void preloadWindow::onLoadClick(GtkWidget *widget, gpointer data) {

  g_print("LOAD\n");
  extern preload_io pre_io;
  extern graph* g;
  extern GtkWidget* scanvas;

  dump* d1 = pre_io.d1;
  preloadWindow* p1 = pre_io.p1;

  GtkWidget** radios = p1->getRadios();

  for (int i=0;i< d1->getValues()->Size();i++ ) {
    if (  gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(radios[i]) ) == TRUE ) {
      g_print("SELECTED:%s\n",d1->getValues()->GetItemName(i).c_str() );
      //replace 200 by entry text
      g = new graph(d1,d1->getValues()->GetItemName(i),200);
      break;
    }
  }

  p1->destroyAll();
  g->setCanvas(scanvas);
  g->draw();

}


void preloadWindow::getWidgets() {
  window = GTK_WIDGET(gtk_builder_get_object (builder, "window1")); 
  progress = GTK_PROGRESS_BAR(gtk_builder_get_object (builder, "progress"));
  label = GTK_LABEL(gtk_builder_get_object(builder,"label"));
  box1 = GTK_BOX(gtk_builder_get_object(builder,"box1"));
  box2 = GTK_BOX(gtk_builder_get_object(builder,"box2"));
  load_btn = GTK_BUTTON(gtk_builder_get_object(builder,"load_btn"));
}

void preloadWindow::showAll() {
  gtk_widget_show_all (window); 
  gtk_widget_set_visible(GTK_WIDGET(box2),FALSE);
}


void preloadWindow::destroyAll() {
  gtk_widget_destroy(GTK_WIDGET(window));

}


GtkProgressBar* preloadWindow::getProgressBar() {
  return progress;
}


GtkWidget** preloadWindow::getRadios() {
  return radios;
}


void preloadWindow::selectIndice(dump* d1) {

  GtkWidget* rbox;
  int nb_indices = d1->getValues()->Size();
  rbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,nb_indices) ;
  radios = (GtkWidget**) malloc(nb_indices * sizeof(GtkWidget*) );

  gtk_widget_destroy(GTK_WIDGET(progress));
  gtk_label_set_text(label,"Please choose the indice you want to work with");

  for (int i=0;i<nb_indices;i++) {

    if (i==0) {
      radios[i] = gtk_radio_button_new_with_label (NULL,d1->getValues()->GetItemName(i).c_str());

    }
    else {
      radios[i] = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON( radios[0] ) ,d1->getValues()->GetItemName(i).c_str());
      g_print("%s\n",d1->getValues()->GetItemName(i).c_str());
    }
    gtk_box_pack_start (GTK_BOX (rbox),GTK_WIDGET( radios[i] ) , TRUE, TRUE, 2);
  }   

  gtk_box_pack_start (box1,rbox , TRUE, TRUE, 2);
  gtk_box_reorder_child(box1,rbox,1);
  

  gtk_window_set_decorated(GTK_WINDOW(window),TRUE);
  gtk_widget_show_all (window);

   g_signal_connect (G_OBJECT (load_btn), "clicked", G_CALLBACK(onLoadClick), NULL);
  

}


gboolean preloadWindow::updateLoadProgress(void* arg) {

  float adv = 0;
  preload_io* pre_io = (preload_io*) arg;
  dump* d1 = pre_io->d1;
  preloadWindow* p1 = pre_io->p1;
  GtkProgressBar* progress = p1->getProgressBar();
  adv = d1->getLoadProgress();
  gtk_progress_bar_set_fraction(progress,adv);
  //g_print("%f\n",adv);
  if (adv >= 1) {
    p1->selectIndice(d1);
    return FALSE;
  }
  else {
    return TRUE;
  }


}


//##########################


void mainWindow::onOpenClick(GtkWidget *widget, gpointer data) {
  g_print("CLick!\n");

  char* dumpfile = NULL;
  extern preload_io pre_io;

  GtkWidget* od =  gtk_file_chooser_dialog_new ("Open Dump File for analysis",
                                                NULL,
                                                GTK_FILE_CHOOSER_ACTION_OPEN,
                                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,NULL);

  if ( gtk_dialog_run(GTK_DIALOG(od)) == GTK_RESPONSE_ACCEPT ) {
    dumpfile = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (od));
    gtk_widget_destroy (od);
  }

  if (dumpfile) {

    preloadWindow* p1 = new preloadWindow();

    pre_io.p1 = p1;
    pre_io.d1 = new dump();
    g_timeout_add(100,preloadWindow::updateLoadProgress,&pre_io);
    pre_io.d1->load_async(dumpfile);

  }


}


mainWindow::mainWindow() {

  builder = gtk_builder_new();
  filename =  g_build_filename (GLADE_WIN, NULL); 

  gtk_builder_add_from_file (builder, filename, &error); 
  g_free (filename); 
  if (error) 
  {
    gint code = error->code; 
    printf("ERROR!\n");
    g_error_free (error); 
  }  

  getWidgets();
  initCanvas();
  showAll();
  connectAll();

}


void mainWindow::getWidgets() {
  window = GTK_WIDGET(gtk_builder_get_object (builder, "window1")); 
  graphbox = GTK_BOX(gtk_builder_get_object (builder, "graphbox"));
  open_btn = GTK_BUTTON(gtk_builder_get_object (builder, "open_btn"));
  //zoomin_btn = GTK_BUTTON(gtk_builder_get_object (builder, "zoomin_btn"));
  //zoomout_btn = GTK_BUTTON(gtk_builder_get_object (builder, "zoomout_btn"));
}


void mainWindow::connectAll() {
  g_signal_connect (G_OBJECT (window), "destroy", (GCallback)gtk_main_quit, NULL); 
  g_signal_connect (G_OBJECT (open_btn), "clicked", G_CALLBACK(onOpenClick), NULL);

}

void mainWindow::showAll() {
  gtk_widget_show_all (window); 
}

void mainWindow::initCanvas() {

  extern GtkWidget* scanvas;
  canvas = goo_canvas_new ();
  scanvas = canvas;
  root = goo_canvas_get_root_item (GOO_CANVAS(canvas));

  gtk_widget_set_size_request (canvas, 800, 400);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 800, 400);
  g_object_set(G_OBJECT(canvas),"background-color-rgb", 0x2A2A2A,NULL);

  gtk_box_pack_start (graphbox, canvas,5,5,0);
  gtk_widget_show (canvas);

}
  
int main(int argc, char *argv []) 

    { 

      gtk_init(&argc, &argv);
      mainWindow* win1 = new mainWindow();
      gtk_main(); 
      return 0; 

    } 