#ifndef CHART_H
#define CHART_H

#include <gtk/gtk.h>
#include "myClass.h"

// context for bar chart: which student to compare vs class average
struct BarCtx {
    myClass *cls;
    int studentIndex;
};

// draws pie chart of letter grade distribution
void drawPieChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data);

// draws bar chart comparing student vs class average
// expects gpointer data to be BarCtx*
void drawBarChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data);

#endif
