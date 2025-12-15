// uicharts.cpp
// Handles showing the bar chart and pie chart in the center display

#include "ui.h"

#include "chart.h"


// shows the pie chart (class averages)
void AppUI::showPie(GtkButton *button, gpointer user_data)
{
    (void)button;
    AppUI *ui = static_cast<AppUI*>(user_data);

    if (!ui->hasClass)
    {
        ui->setLabel(ui->fileText, "Load a scores file first.");
        return;
    }

    // drawing area where the chart is rendered
    GtkWidget *area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(area, TRUE);
    gtk_widget_set_vexpand(area, TRUE);

    gtk_drawing_area_set_draw_func(
        GTK_DRAWING_AREA(area),
        drawPieChart,
        &ui->cls,
        NULL
    );

    // frame keeps the same look as the text display area
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_hexpand(frame, TRUE);
    gtk_widget_set_vexpand(frame, TRUE);
    gtk_widget_add_css_class(frame, "display-area");
    gtk_frame_set_child(GTK_FRAME(frame), area);

    ui->swapCenter(frame);
    gtk_widget_queue_draw(area);
}


// shows the bar chart (student vs class average)
void AppUI::showBar(GtkButton *button, gpointer user_data)
{
    (void)button;
    AppUI *ui = static_cast<AppUI*>(user_data);

    if (!ui->hasClass || ui->barCtx.cls == NULL)
    {
        ui->setLabel(ui->fileText, "Load a scores file first.");
        return;
    }

    GtkWidget *area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(area, TRUE);
    gtk_widget_set_vexpand(area, TRUE);

    gtk_drawing_area_set_draw_func(
        GTK_DRAWING_AREA(area),
        drawBarChart,
        &ui->barCtx,
        NULL
    );

    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_hexpand(frame, TRUE);
    gtk_widget_set_vexpand(frame, TRUE);
    gtk_widget_add_css_class(frame, "display-area");
    gtk_frame_set_child(GTK_FRAME(frame), area);

    ui->swapCenter(frame);
    gtk_widget_queue_draw(area);
}

