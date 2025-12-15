#include <gtk/gtk.h>
#include "ui.h"


// starts the app window
static void activate(GtkApplication *app, gpointer user_data)
{
    (void)user_data;

    static AppUI ui;
    ui.run(app);
}


int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.gradecalc", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
