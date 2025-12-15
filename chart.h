#ifndef CHART_H
#define CHART_H

#include <gtk/gtk.h>
#include "myClass.h"

// draws pie chart of letter grade distribution
void drawPieChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data);

// draws bar chart comparing student vs class average
void drawBarChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data);

#endif
