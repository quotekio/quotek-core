#include <gtk/gtk.h> 
#include <goocanvas-2.0/goocanvas.h>
#include <stdio.h>
#include "dump.h"
#include "graph.h"
#include <pthread.h>

#define GLADE_WIN "/var/www/hexosam/adam/src/signal/glade/main.glade"
#define GLADE_PRELOAD_WIN "/var/www/hexosam/adam/src/signal/glade/preload.glade"

class preloadWindow {

  public:
    preloadWindow();
    void getWidgets();
    void showAll();
    void connectAll();
    void destroyAll();


    GtkProgressBar* getProgressBar();
    GtkWidget** getRadios();
    static gboolean updateLoadProgress(void*);
    void selectIndice(dump* d1);

    /* Event Callbacks */
    static void onLoadClick(GtkWidget*, gpointer);

  private:
    GtkBuilder *builder = NULL; 
    GtkWidget *window = NULL;
    GtkProgressBar *progress = NULL;
    GtkLabel *label = NULL;
    GtkBox* box1 = NULL;
    GtkBox* box2 = NULL;
    GtkButton* load_btn = NULL; 
    GError *error = NULL; 
    gchar *filename = NULL;
    GtkWidget** radios;

};

class mainWindow {
  public:
    mainWindow();
    void getWidgets();
    void showAll();
    void connectAll();
    void initCanvas();
    static void onOpenClick(GtkWidget*, gpointer);

  private:

    GtkBuilder *builder = NULL; 
    GtkWidget *window = NULL; 
    GtkBox *graphbox = NULL;

    GtkButton* open_btn = NULL;
    GtkButton* zoomin_btn = NULL;
    GtkButton* zoomout_btn = NULL;

    GtkWidget* canvas = NULL;
    GError *error = NULL; 
    gchar *filename = NULL;
    GooCanvasItem* root;
};


typedef struct preload_io {
  preloadWindow* p1;
  dump* d1;
} preload_io;
