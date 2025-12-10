#include <gtk/gtk.h>
#include <string>
#include <fstream>
#include "authentication.h"

// pointer to textview which will show the file and whats inside of it
static GtkWidget *fileview = NULL;

// This function pops up a login window.
// It keeps asking for a username and password until:
//   1) the user enters valid info  -> returns true
//   2) the user hits Cancel / closes the dialog -> returns false
bool showlogindialog(GtkWindow *parent)
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
static void clickbutton(GtkButton *btn, gpointer data)
{
    g_print("Button was clicked!\n");
}

// callback for the "Open Scores File" button
static void openfile(GtkButton *btn, gpointer data)
{
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Open Scores File",
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT,
        NULL);

    gint res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT)
    {
        // get the selected file path
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // read file into a C++ string
        std::ifstream file(filename);
        std::string line, contents;

        if (file)
        {
            while (std::getline(file, line))
            {
                contents += line + "\n";
            }
        }
        else
        {
            contents = "Error: Could not open file.";
        }

        // show file contents in the TextView
        if (fileview != NULL)
        {
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(fileview));
            gtk_text_buffer_set_text(buffer, contents.c_str(), -1);
        }

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

// This is the startup function for the GTK app.
// It creates the main window and sets up the widgets inside it.
static void startapp(GtkApplication *app, gpointer data)
{
    // create the main window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grade Calculator Practice");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    // before showing the main window, force the user to log in
    if (!showlogindialog(GTK_WINDOW(window)))
    {
        g_application_quit(G_APPLICATION(app));
        return;
    }

    // main horizontal box (sidebar on left, content on right)
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 20);

    // left sidebar
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // button to open a scores file
    GtkWidget *openbtn = gtk_button_new_with_label("Open Scores File");
    g_signal_connect(openbtn, "clicked", G_CALLBACK(openfile), NULL);
    gtk_box_pack_start(GTK_BOX(sidebar), openbtn, FALSE, FALSE, 5);

    // your original button (kept)
    GtkWidget *button = gtk_button_new_with_label("Click Me");
    g_signal_connect(button, "clicked", G_CALLBACK(clickbutton), NULL);
    gtk_box_pack_start(GTK_BOX(sidebar), button, FALSE, FALSE, 5);

    // add sidebar to main hbox
    gtk_box_pack_start(GTK_BOX(hbox), sidebar, FALSE, FALSE, 10);

    // right side content area
    GtkWidget *contentbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // text view for showing file contents
    fileview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(fileview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(fileview), FALSE);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), fileview);

    gtk_box_pack_start(GTK_BOX(contentbox), scroll, TRUE, TRUE, 5);

    // combine contentbox into the main hbox
    gtk_box_pack_start(GTK_BOX(hbox), contentbox, TRUE, TRUE, 10);

    // put everything in the window
    gtk_container_add(GTK_CONTAINER(window), hbox);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app;  // pointer to the GTK application object
    int status;           // used to store the return code

    // create a new GTK application with a ID
    app = gtk_application_new("com.example.gradecalc", G_APPLICATION_FLAGS_NONE);

    // tell GTK which function to call when the app starts/activates
    g_signal_connect(app, "activate", G_CALLBACK(startapp), NULL);

    // start the main GTK loop (this keeps the windows on the screen)
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // clean up after the app closes
    g_object_unref(app);

    return status;        // return the apps status code to the OS
}
