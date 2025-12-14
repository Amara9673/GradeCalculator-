#include <gtk/gtk.h>
#include <string>
#include <fstream>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "authentication.h"
#include <cstring>


// switch between the login screen and the main app screen
static GtkStack  *stack = NULL;
static GtkWidget *loginpage = NULL;
static GtkWidget *mainpage  = NULL;

// for avatar stuff
static GtkWidget *avatarframe = NULL;  // outline box
static GtkWidget *avatarimg = NULL;    // actual image
static GtkWidget *avatartxt = NULL;    // shows "empty" text when no avatar
static std::string avatarpath = "";
static const int AVATAR_BOX = 120;   // how big the avatar shows up on screen


// for the file stuff
static GtkWidget *fileview = NULL;    // big text box on the right
static GtkWidget *filelabel = NULL;   // shows what file we selected
static std::string currentfile = "";

// quick helper so i dont repeat this everywhere
static void setview(const std::string& text)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(fileview));

    // gtk text buffers need valid utf-8 or it throws that critical error
    if (g_utf8_validate(text.c_str(), text.size(), NULL))
    {
        gtk_text_buffer_set_text(buf, text.c_str(), -1);
    }
    else
    {
        // if the file has weird bytes, just force it into valid utf-8
        // (this keeps the program from crashing or spamming errors)
        char *fixed = g_strdup(text.c_str());
        g_strcanon(fixed, (const char *)" -_.,;:!?()[]{}<>/\\|@#$%^&*+=~`\"'\n\r\t"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", '?');
        gtk_text_buffer_set_text(buf, fixed, -1);
        g_free(fixed);
    }
}


// just updates the little status message on the login page
static void set_status(GtkWidget *label, const char *msg)
{
    gtk_label_set_text(GTK_LABEL(label), msg);
}

// this runs after the file picker closes (only when a file was actually picked)
static void filepicked(GObject *source, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

    // user canceled out of the file chooser
    if (!file)
        return;

    char *path = g_file_get_path(file);
    if (!path)
    {
        g_object_unref(file);
        return;
    }

    currentfile = path;
        // basic check so we dont try to display random binary files
    if (currentfile.size() < 4 || currentfile.substr(currentfile.size() - 4) != ".txt")
    {
        gtk_label_set_text(GTK_LABEL(filelabel), "Pick a .txt scores file");
        setview("that file is not a .txt\n");
        g_free(path);
        g_object_unref(file);
        return;
    }


    // show the file path so its obvious what we picked
    std::string labeltxt = "Selected: " + currentfile;
    gtk_label_set_text(GTK_LABEL(filelabel), labeltxt.c_str());

    // open the file and dump it into the text box
    std::ifstream in(currentfile.c_str());
    std::string line;
    std::string contents;

    if (in)
    {
        while (std::getline(in, line))
            contents += line + "\n";
    }
    else
    {
        contents = "could not open that file\n";
    }

    setview(contents);

    g_free(path);
    g_object_unref(file);
}

// runs when the user clicks "Choose File"
static void choosefile(GtkButton *btn, gpointer data)
{
    GtkWindow *win = GTK_WINDOW(data);

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Choose a scores file");

    // only show text files so nobody accidentally opens an image
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_set_name(filter, "Text files (*.txt)");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, filter);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));

    g_object_unref(filter);
    g_object_unref(filters);

    gtk_file_dialog_open(dialog, win, NULL, filepicked, NULL);
}


// runs after the user picks an avatar image
static void avatarpicked(GObject *source, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

    // user canceled
    if (!file)
        return;

    char *path = g_file_get_path(file);
    if (!path)
    {
        g_object_unref(file);
        return;
    }

    avatarpath = path;

    // try to load it as an image and scale it into the avatar box
    GError *err = NULL;
    GdkPixbuf *pix = gdk_pixbuf_new_from_file(path, &err);

    if (!pix)
    {
        // not an image / couldnt load
        gtk_label_set_text(GTK_LABEL(avatartxt), "could not load image");
        if (err) g_error_free(err);

        g_free(path);
        g_object_unref(file);
        return;
    }

    GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pix, AVATAR_BOX, AVATAR_BOX, GDK_INTERP_BILINEAR);

    if (scaled)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(avatarimg), scaled);
        g_object_unref(scaled);
    }
    else
    {
        // fallback just in case
        gtk_image_set_from_file(GTK_IMAGE(avatarimg), avatarpath.c_str());
    }

    // swap the frame from the placeholder text to the actual image
    gtk_frame_set_child(GTK_FRAME(avatarframe), avatarimg);

    g_object_unref(pix);
    g_free(path);
    g_object_unref(file);
}


// runs when the user clicks "Upload Avatar"
static void chooseavatar(GtkButton *btn, gpointer data)
{
    GtkWindow *win = GTK_WINDOW(data);

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Choose an avatar image");
    gtk_file_dialog_open(dialog, win, NULL, avatarpicked, NULL);
}

// when the user clicks login, check the username/password
static void on_login_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget **widgets = (GtkWidget **)user_data;

    GtkWidget *user_entry = widgets[0];
    GtkWidget *pass_entry = widgets[1];
    GtkWidget *status_lbl = widgets[2];

    const char *u = gtk_editable_get_text(GTK_EDITABLE(user_entry));
    const char *p = gtk_editable_get_text(GTK_EDITABLE(pass_entry));

    std::string username = u ? u : "";
    std::string password = p ? p : "";

    if (loginCheck(username, password))
    {
        set_status(status_lbl, "Login successful.");
        gtk_stack_set_visible_child(GTK_STACK(stack), mainpage);
    }
    else
    {
        set_status(status_lbl, "Invalid username or password.");
    }
}

// just a placeholder button so you know clicks work
static void on_button_clicked(GtkButton *button, gpointer user_data)
{
    g_print("Button was clicked!\n");
}

// startup function for the GTK app
static void activate(GtkApplication *app, gpointer user_data)
{
    // main window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grade Calculator Practice");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);

    // stack holds login page + main page
    stack = GTK_STACK(gtk_stack_new());
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(stack));

    // login page
    loginpage = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(loginpage, 20);
    gtk_widget_set_margin_bottom(loginpage, 20);
    gtk_widget_set_margin_start(loginpage, 20);
    gtk_widget_set_margin_end(loginpage, 20);

    GtkWidget *title = gtk_label_new("Please login:");
    gtk_box_append(GTK_BOX(loginpage), title);

    GtkWidget *user_label = gtk_label_new("Username:");
    gtk_box_append(GTK_BOX(loginpage), user_label);

    GtkWidget *user_entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(loginpage), user_entry);

    GtkWidget *pass_label = gtk_label_new("Password:");
    gtk_box_append(GTK_BOX(loginpage), pass_label);

    GtkWidget *pass_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(pass_entry), '*');
    gtk_box_append(GTK_BOX(loginpage), pass_entry);

    GtkWidget *status_lbl = gtk_label_new("");
    gtk_box_append(GTK_BOX(loginpage), status_lbl);

    GtkWidget *login_btn = gtk_button_new_with_label("Login");
    gtk_box_append(GTK_BOX(loginpage), login_btn);

    // pass widgets into the login callback
    static GtkWidget *login_widgets[3];
    login_widgets[0] = user_entry;
    login_widgets[1] = pass_entry;
    login_widgets[2] = status_lbl;

    g_signal_connect(login_btn, "clicked", G_CALLBACK(on_login_clicked), login_widgets);

    // main page
    mainpage = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_top(mainpage, 20);
    gtk_widget_set_margin_bottom(mainpage, 20);
    gtk_widget_set_margin_start(mainpage, 20);
    gtk_widget_set_margin_end(mainpage, 20);

    // left side buttons
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *choosebtn = gtk_button_new_with_label("Choose File");
    g_signal_connect(choosebtn, "clicked", G_CALLBACK(choosefile), window);
    gtk_box_append(GTK_BOX(sidebar), choosebtn);

    GtkWidget *avatarbtn = gtk_button_new_with_label("Upload Avatar");
    g_signal_connect(avatarbtn, "clicked", G_CALLBACK(chooseavatar), window);
    gtk_box_append(GTK_BOX(sidebar), avatarbtn);

    // placeholders so it looks like an actual app menu
    GtkWidget *sortbtn = gtk_button_new_with_label("Sorting");
    gtk_box_append(GTK_BOX(sidebar), sortbtn);

    GtkWidget *barchartbtn = gtk_button_new_with_label("Bar Chart");
    gtk_box_append(GTK_BOX(sidebar), barchartbtn);

    GtkWidget *piebtn = gtk_button_new_with_label("Pie Chart");
    gtk_box_append(GTK_BOX(sidebar), piebtn);

    GtkWidget *testbtn = gtk_button_new_with_label("Click Me");
    g_signal_connect(testbtn, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_box_append(GTK_BOX(sidebar), testbtn);

    gtk_box_append(GTK_BOX(mainpage), sidebar);

    // right side content
    GtkWidget *right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // top row: file label left, avatar right
    GtkWidget *toprow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(right), toprow);

    filelabel = gtk_label_new("Selected: (none)");
    gtk_widget_set_hexpand(filelabel, TRUE);              // label takes the extra space
    gtk_widget_set_halign(filelabel, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(toprow), filelabel);

    // avatar box (fixed size, top-right)
    avatarframe = gtk_frame_new(NULL);
    gtk_widget_set_size_request(avatarframe, 120, 120);
    gtk_widget_set_hexpand(avatarframe, FALSE);
    gtk_widget_set_vexpand(avatarframe, FALSE);
    gtk_widget_set_halign(avatarframe, GTK_ALIGN_END);
    gtk_widget_set_valign(avatarframe, GTK_ALIGN_START);

    avatartxt = gtk_label_new("no avatar");
    gtk_frame_set_child(GTK_FRAME(avatarframe), avatartxt);

    avatarimg = gtk_image_new();
    gtk_widget_set_size_request(avatarimg, 120, 120);     // helps it not look tiny in a big area
    gtk_box_append(GTK_BOX(toprow), avatarframe);

    // big text box
    fileview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(fileview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(fileview), FALSE);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), fileview);

    gtk_box_append(GTK_BOX(right), scroll);
    gtk_box_append(GTK_BOX(mainpage), right);

    // add both pages to the stack and show login first
    gtk_stack_add_named(GTK_STACK(stack), loginpage, "login");
    gtk_stack_add_named(GTK_STACK(stack), mainpage, "main");
    gtk_stack_set_visible_child(GTK_STACK(stack), loginpage);

    gtk_widget_show(window);
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
