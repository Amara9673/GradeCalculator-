#include <cairo.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "chart.h"

// ------------------ small helper ------------------
static void draw_text(cairo_t *cr, double x, double y, const std::string& s)
{
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, s.c_str());
}

static void fill_background_white(cairo_t *cr)
{
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
}

// ---------------- PIE CHART ----------------
void drawPieChart(GtkDrawingArea *area,
                  cairo_t *cr,
                  int width,
                  int height,
                  gpointer data)
{
    (void)area;

    myClass *cls = (myClass*)data;
    if (!cls) return;

    int n = cls->getStudentsLength();
    if (n <= 0) return;

    std::vector<double> perc = cls->getPercentLetter();
    if (perc.size() < 5) return;

    fill_background_white(cr);

    // Geometry
    double cx = width * 0.35;
    double cy = height * 0.50;
    double radius = std::min(width, height) * 0.28;

    // Colors for A B C D F
    double colors[5][3] = {
        {0.20, 0.60, 0.20},  // A
        {0.20, 0.40, 0.80},  // B
        {0.90, 0.70, 0.20},  // C
        {0.90, 0.40, 0.20},  // D
        {0.70, 0.20, 0.20}   // F
    };

    const char *labels[5] = {"A", "B", "C", "D", "F"};

    // Draw title
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    draw_text(cr, 20, 22, "Grade Distribution (A–F)");

    // Draw slices
    cairo_set_line_width(cr, 1.0);

    // Start at -90 degrees so first slice begins at top
    double angle = -G_PI / 2.0;

    for (int i = 0; i < 5; i++)
    {
        double slice = (perc[i] / 100.0) * 2.0 * G_PI;

        // skip zero slices to avoid tiny artifacts
        if (slice <= 0.0) continue;

        cairo_set_source_rgb(cr, colors[i][0], colors[i][1], colors[i][2]);
        cairo_move_to(cr, cx, cy);
        cairo_arc(cr, cx, cy, radius, angle, angle + slice);
        cairo_close_path(cr);
        cairo_fill(cr);

        // slice border
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_move_to(cr, cx, cy);
        cairo_arc(cr, cx, cy, radius, angle, angle + slice);
        cairo_close_path(cr);
        cairo_stroke(cr);

        angle += slice;
    }

    // Legend (right side)
    double lx = width * 0.70;
    double ly = height * 0.28;

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);
    cairo_set_source_rgb(cr, 0, 0, 0);

    for (int i = 0; i < 5; i++)
    {
        // color box
        cairo_set_source_rgb(cr, colors[i][0], colors[i][1], colors[i][2]);
        cairo_rectangle(cr, lx, ly + i * 24, 14, 14);
        cairo_fill(cr);

        // text
        cairo_set_source_rgb(cr, 0, 0, 0);
        std::ostringstream oss;
        oss << labels[i] << ": " << std::fixed << std::setprecision(1) << perc[i] << "%";
        draw_text(cr, lx + 22, ly + i * 24 + 12, oss.str());
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

    // Expect BarCtx from chart.h:
    // struct BarCtx { myClass *cls; int studentIndex; };
    BarCtx *ctx = (BarCtx*)data;
    if (!ctx || !ctx->cls) return;

    myClass *cls = ctx->cls;

    int n = cls->getStudentsLength();
    if (n <= 0) return;

    int idx = ctx->studentIndex;
    if (idx < 0) idx = 0;
    if (idx >= n) idx = n - 1;

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

    const char *names[5] = {"Labs", "Quizzes", "Midterms", "Projects", "Final"};

    fill_background_white(cr);

    // Plot margins
    double left   = 60;
    double right  = 20;
    double top    = 40;
    double bottom = 55;

    double plotW = width - left - right;
    double plotH = height - top - bottom;

    if (plotW <= 50 || plotH <= 50)
        return;

    // Title
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    draw_text(cr, left, 22, "Student vs Class Average (Category Percent)");

    // Axes
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, left, top);
    cairo_line_to(cr, left, top + plotH);
    cairo_line_to(cr, left + plotW, top + plotH);
    cairo_stroke(cr);

    // Grid + y-axis labels: 0, 25, 50, 75, 100
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    for (int p = 0; p <= 100; p += 25)
    {
        double y = top + plotH - (p / 100.0) * plotH;

        // grid line
        cairo_set_source_rgb(cr, 0.88, 0.88, 0.88);
        cairo_move_to(cr, left, y);
        cairo_line_to(cr, left + plotW, y);
        cairo_stroke(cr);

        // label
        cairo_set_source_rgb(cr, 0, 0, 0);
        std::ostringstream oss;
        oss << p << "%";
        draw_text(cr, 12, y + 4, oss.str());
    }

    // Bar layout: 5 groups, 2 bars each
    int groups = 5;
    double groupW = plotW / groups;
    double barW = groupW * 0.28;
    double gap  = groupW * 0.10;

    for (int i = 0; i < groups; i++)
    {
        double gx = left + i * groupW;

        // class bar (gray)
        double h1 = (classVals[i] / 100.0) * plotH;
        if (h1 < 0) h1 = 0;
        if (h1 > plotH) h1 = plotH;

        cairo_set_source_rgb(cr, 0.65, 0.65, 0.65);
        cairo_rectangle(cr, gx + gap, top + plotH - h1, barW, h1);
        cairo_fill(cr);

        // student bar (blue-ish)
        double h2 = (studentVals[i] / 100.0) * plotH;
        if (h2 < 0) h2 = 0;
        if (h2 > plotH) h2 = plotH;

        cairo_set_source_rgb(cr, 0.20, 0.40, 0.80);
        cairo_rectangle(cr, gx + gap + barW + gap, top + plotH - h2, barW, h2);
        cairo_fill(cr);

        // numeric labels above bars
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_font_size(cr, 11);

        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << classVals[i] << "%";
            draw_text(cr, gx + gap, top + plotH - h1 - 6, oss.str());
        }
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << studentVals[i] << "%";
            draw_text(cr, gx + gap + barW + gap, top + plotH - h2 - 6, oss.str());
        }

        // category label below axis
        cairo_set_font_size(cr, 12);
        draw_text(cr, gx + 6, top + plotH + 30, names[i]);
    }

    // Legend (top-right inside plot)
    double legX = left + plotW - 165;
    double legY = top + 8;

    cairo_set_font_size(cr, 12);

    // Class avg legend
    cairo_set_source_rgb(cr, 0.65, 0.65, 0.65);
    cairo_rectangle(cr, legX, legY, 12, 12);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    draw_text(cr, legX + 18, legY + 11, "Class Avg");

    // Student legend
    cairo_set_source_rgb(cr, 0.20, 0.40, 0.80);
    cairo_rectangle(cr, legX, legY + 18, 12, 12);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    draw_text(cr, legX + 18, legY + 29, "Student");
}
