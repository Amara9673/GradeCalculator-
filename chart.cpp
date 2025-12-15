#include <cairo.h>
#include <vector>
#include <string>

#include "chart.h"

// ---------------- PIE CHART ----------------
void drawPieChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data)
{
    (void)area;

    myClass *cls = (myClass*)data;
    if(cls->getStudentsLength() == 0)
        return;

    std::vector<double> perc = cls->getPercentLetter();

    double cx = width / 2.0;
    double cy = height / 2.0;
    double radius = (width < height ? width : height) / 3.0;

    double angle = 0.0;

    // simple colors for A B C D F
    double colors[5][3] = {
        {0.2, 0.6, 0.2},  // A
        {0.2, 0.4, 0.8},  // B
        {0.9, 0.7, 0.2},  // C
        {0.9, 0.4, 0.2},  // D
        {0.7, 0.2, 0.2}   // F
    };

    for(int i = 0; i < 5; i++)
    {
        double slice = perc[i] / 100.0 * 2.0 * G_PI;

        cairo_set_source_rgb(cr,
                              colors[i][0],
                              colors[i][1],
                              colors[i][2]);

        cairo_move_to(cr, cx, cy);
        cairo_arc(cr, cx, cy, radius, angle, angle + slice);
        cairo_close_path(cr);
        cairo_fill(cr);

        angle += slice;
    }
}

// ---------------- BAR CHART ----------------
void drawBarChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data)
{
    (void)area;

    myClass *cls = (myClass*)data;
    if(cls->getStudentsLength() == 0)
        return;

    int idx = 0; // first student

    std::vector<double> studentVals = {
        cls->getLabScore(idx),
        cls->getQuizScore(idx),
        cls->getMidtermScore(idx),
        cls->getProjectScore(idx),
        cls->getFinalScore(idx)
    };

    std::vector<double> classVals = {
        cls->getClassLabScore(),
        cls->getClassQuizScore(),
        cls->getClassMidtermScore(),
        cls->getClassProjectScore(),
        cls->getClassFinalScore()
    };

    int bars = 5;
    double barWidth = width / (bars * 3.0);
    double maxH = height - 40;

    for(int i = 0; i < bars; i++)
    {
        double x = (i * 3 + 1) * barWidth;

        // class average (gray)
        cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
        double h1 = (classVals[i] / 100.0) * maxH;
        cairo_rectangle(cr, x, height - h1, barWidth, h1);
        cairo_fill(cr);

        // student (blue)
        cairo_set_source_rgb(cr, 0.2, 0.4, 0.8);
        double h2 = (studentVals[i] / 100.0) * maxH;
        cairo_rectangle(cr, x + barWidth, height - h2, barWidth, h2);
        cairo_fill(cr);
    }
}
