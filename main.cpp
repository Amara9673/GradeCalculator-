#include <gtk/gtk.h>
#include <string>
#include "authentication.h"

// This function pops up a login window.
// It keeps asking for a username and password until:
//   1) the user enters valid info  -> returns true
//   2) the user hits Cancel / closes the dialog -> returns false
bool show_login_dialog(GtkWindow *parent)
{
    bool loggedIn = false;   // will become true only if loginCheck passes
    bool done     = false;   // controls the while loop

    while (!done)
    {
        // create the login dialog window with two buttons: Cancel and Login
        GtkWidget *dialog = gtk_dialog_new_with_buttons(
            "Login",                 // title of the window
            parent,                  // parent window (can be main window)
            GTK_DIALOG_MODAL,        // makes the dialog block input to other windows
            "_Cancel",               // text for the Cancel button
            GTK_RESPONSE_CANCEL,     // value returned when Cancel is clicked
            "_Login",                // text for the Login button
            GTK_RESPONSE_OK,         // value returned when Login is clicked
            NULL                     // end of the button list
        );

        // get the area inside the dialog where we can pack widgets
        GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        // create a vertical box to stack the labels and text entries
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
        gtk_box_pack_start(GTK_BOX(content), vbox, TRUE, TRUE, 0);

        //  username label + text box 
        GtkWidget *user_label = gtk_label_new("Username:");
        gtk_box_pack_start(GTK_BOX(vbox), user_label, FALSE, FALSE, 0);

        GtkWidget *user_entry = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(vbox), user_entry, FALSE, FALSE, 0);

        //  password label + text box 
        GtkWidget *pass_label = gtk_label_new("Password:");
        gtk_box_pack_start(GTK_BOX(vbox), pass_label, FALSE, FALSE, 0);

        GtkWidget *pass_entry = gtk_entry_new();
        // hide the actual characters so it looks like a real password field
        gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
        gtk_entry_set_invisible_char(GTK_ENTRY(pass_entry), '*');
        gtk_box_pack_start(GTK_BOX(vbox), pass_entry, FALSE, FALSE, 0);

        // make all the widgets in the dialog visible
        gtk_widget_show_all(dialog);

        // run the dialog and wait until the user presses a button
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));

        // if the user hit cancel/closed the window then it will stop asking to log in
        if (response == GTK_RESPONSE_CANCEL || response == GTK_RESPONSE_DELETE_EVENT)
        {
            gtk_widget_destroy(dialog);
            loggedIn = false;
            done = true;       // leave the while loop
        }
        else if (response == GTK_RESPONSE_OK)
        {
            // grab whatever the user typed into the two text boxes
            const char *u = gtk_entry_get_text(GTK_ENTRY(user_entry));
            const char *p = gtk_entry_get_text(GTK_ENTRY(pass_entry));

            std::string username = u;
            std::string password = p;

            gtk_widget_destroy(dialog);

            // use our authentication function to check the login info
            if (loginCheck(username, password))
            {
                // show a login successful message
                GtkWidget *msg = gtk_message_dialog_new(
                    parent,
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_INFO,
                    GTK_BUTTONS_OK,
                    "Login successful. Welcome back, %s!",
                    username.c_str()
                );
                gtk_dialog_run(GTK_DIALOG(msg));
                gtk_widget_destroy(msg);

                loggedIn = true;   // mark that the user is now logged in
                done = true;       // leave the while loop
            }
            else
            {
                // if the username/password combo was not found in the file tell the user and then the while loop will repeat
                GtkWidget *msg = gtk_message_dialog_new(
                    parent,
                    GTK_DIALOG_MODAL,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Invalid username or password."
                );
                gtk_dialog_run(GTK_DIALOG(msg));
                gtk_widget_destroy(msg);

                // loggedIn stays false, done stays false & we loop again
            }
        }
        else
        {
            // any other response 
            gtk_widget_destroy(dialog);
            loggedIn = false;
            done = true;
        }
    }

    return loggedIn;
}

// This function runs when the button in the main window gets clicked.
// Right now it just prints a message to the terminal.
static void on_button_clicked(GtkButton *button, gpointer user_data)
{
    g_print("Button was clicked!\n");
}

// This is the startup function for the GTK app.
// It creates the main window and sets up the widgets inside it.
static void activate(GtkApplication *app, gpointer user_data)
{
    // create the main window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grade Calculator Practice");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // before showing the main window, force the user to log in
    if (!show_login_dialog(GTK_WINDOW(window)))
    {
        // if show_login_dialog returned false, user failed/exited so we just quit the program
        // i think we could put something funny here. like an image or smthn when they get the login wrong
        g_application_quit(G_APPLICATION(app));
        return;
    }

    // main vertical box to organize everything inside the window
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);

    // simple label at the top of the window
    GtkWidget *label = gtk_label_new("Welcome to Grade Calculator");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

    // button the user can click (for now it just prints to the terminal)
    GtkWidget *button = gtk_button_new_with_label("Click Me");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);

    // actually put the vbox into the window and show everything
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;  // pointer to the GTK application object
    int status;           // used to store the return code

    // create a new GTK application with a simple app ID
    app = gtk_application_new("com.example.gradecalc", G_APPLICATION_FLAGS_NONE);

    // tell GTK which function to call when the app starts/activates
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // start the main GTK loop (this keeps the windows on the screen)
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // clean up after the app closes
    g_object_unref(app);

    return status;        // return the apps status code to the OS
}
