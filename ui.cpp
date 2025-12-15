#include "ui.h"
#include "load.h"

#include <fstream>
#include <sstream>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "authentication.h"
#include "theme.h"
#include "chart.h"

#include <iostream>


AppUI::AppUI()
{
    hasClass = false;

    // starting values
    window   = NULL;
    stack    = NULL;
    loginBox = NULL;
    mainBox  = NULL;

    // login widgets
    userIn   = NULL;
    passIn   = NULL;
    loginMsg = NULL;

    // avatar stuff
    avatarFrame  = NULL;
    avatarImg    = NULL;
    noAvatarText = NULL;
    avatarFile   = "";

    // file display stuff
    textBox   = NULL;
    fileText  = NULL;
    curFile   = "";

    // center swap support (text scroll <-> chart)
    centerWidget = NULL;     // current center widget (scroll or chart frame)
    rightBox     = NULL;     // container holding the top row + center widget
    textScroll   = NULL;     // persistent scrolled window holding textBox

    // bar chart context
    barCtx.cls = NULL;
    barCtx.studentIndex = 0;
}


// updates a label with a message
void AppUI::setLabel(GtkWidget *label, const char *msg)
{
    if (label != NULL)
        gtk_label_set_text(GTK_LABEL(label), msg);
}


// writes text to the center display box (GTK needs UTF-8)
void AppUI::setText(const std::string& text)
{
    if (textBox == NULL) return;

    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textBox));

    if (g_utf8_validate(text.c_str(), -1, NULL))
        gtk_text_buffer_set_text(buf, text.c_str(), -1);
    else
        gtk_text_buffer_set_text(buf, "could not display file\n", -1);
}


// helper: swap the center widget inside rightBox
void AppUI::swapCenter(GtkWidget *newCenter)
{
    if (rightBox == NULL) return;
    if (newCenter == NULL) return;
    if (!GTK_IS_WIDGET(newCenter)) return;

    // If newCenter already has a parent, handle it safely
    GtkWidget *newParent = gtk_widget_get_parent(newCenter);

    // If it's already the active center widget, do nothing
    if (newCenter == centerWidget && newParent == rightBox)
        return;

    // Remove current center only if it is actually inside rightBox
    if (centerWidget != NULL && GTK_IS_WIDGET(centerWidget))
    {
        GtkWidget *curParent = gtk_widget_get_parent(centerWidget);
        if (curParent == rightBox)
            gtk_box_remove(GTK_BOX(rightBox), centerWidget);
    }

    // If newCenter is parented somewhere else, remove it from that container first
    if (newParent != NULL && newParent != rightBox)
    {
        if (GTK_IS_BOX(newParent))
            gtk_box_remove(GTK_BOX(newParent), newCenter);
        else
            gtk_widget_unparent(newCenter);
    }

    // If it's not already in rightBox, append it
    newParent = gtk_widget_get_parent(newCenter);
    if (newParent != rightBox)
        gtk_box_append(GTK_BOX(rightBox), newCenter);

    centerWidget = newCenter;
}



// open file dialog
void AppUI::pickFile(GtkButton *btn, gpointer data)
{
    (void)btn;

    AppUI *ui = static_cast<AppUI*>(data);

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Choose a scores file");

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_set_name(filter, "Text files (*.txt)");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, filter);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));

    g_object_unref(filter);
    g_object_unref(filters);

    gtk_file_dialog_open(dialog, GTK_WINDOW(ui->window), NULL, AppUI::pickedFile, ui);
    g_object_unref(dialog);
}


// runs after user picks a file (or cancels)
void AppUI::pickedFile(GObject *source, GAsyncResult *res, gpointer data)
{
    AppUI *ui = static_cast<AppUI*>(data);

    GFile *file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source), res, NULL);
    if (file == NULL) return; // user cancelled

    char *path = g_file_get_path(file);
    g_object_unref(file);
    if (path == NULL) return;

    ui->curFile = path;
    g_free(path);

    // only allow .txt
    if (ui->curFile.size() < 4 ||
        ui->curFile.substr(ui->curFile.size() - 4) != ".txt")
    {
        ui->setLabel(ui->fileText, "Pick a .txt scores file");
        ui->setText("that file is not a .txt\n");
        ui->hasClass = false;

        ui->barCtx.cls = NULL;
        ui->barCtx.studentIndex = 0;
        return;
    }

    std::string labeltxt = "Selected: " + ui->curFile;
    ui->setLabel(ui->fileText, labeltxt.c_str());

    // read file into display
    std::ifstream in(ui->curFile.c_str());
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

    ui->setText(contents);

    // load class data
    myClass temp;
    if (loadScoresFile(ui->curFile, temp))
    {
        ui->cls = temp;
        ui->hasClass = true;

        // bar chart context now points to loaded class
        ui->barCtx.cls = &ui->cls;
        ui->barCtx.studentIndex = 0; // default: first student

        std::cout << "Students: " << ui->cls.getStudentsLength() << std::endl;
        std::cout << "Class lab avg: " << ui->cls.getClassLabScore() << std::endl;
    }
    else
    {
        ui->hasClass = false;
        ui->barCtx.cls = NULL;
        ui->barCtx.studentIndex = 0;
    }

    // go back to the text display area after loading a new file
    if (ui->textScroll != NULL)
    {
        ui->swapCenter(ui->textScroll);
    }

}


// open avatar dialog
void AppUI::pickAvatar(GtkButton *btn, gpointer data)
{
    (void)btn;

    AppUI *ui = static_cast<AppUI*>(data);

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Choose an avatar image");
    gtk_file_dialog_open(dialog, GTK_WINDOW(ui->window), NULL, AppUI::pickedAvatar, ui);
    g_object_unref(dialog);
}


// loads avatar, scales it, then puts it in the frame
void AppUI::pickedAvatar(GObject *source, GAsyncResult *res, gpointer data)
{
    AppUI *ui = static_cast<AppUI*>(data);

    GFile *file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source), res, NULL);
    if (file == NULL) return; // user cancelled

    char *path = g_file_get_path(file);
    g_object_unref(file);
    if (path == NULL) return;

    ui->avatarFile = path;

    // load image
    GError *err = NULL;
    GdkPixbuf *pix = gdk_pixbuf_new_from_file(path, &err);

    if (!pix)
    {
        ui->setLabel(ui->noAvatarText, "could not load image");
        if (err) g_error_free(err);
        g_free(path);
        return;
    }

    // scale to square
    GdkPixbuf *scaled =
        gdk_pixbuf_scale_simple(pix, AVATAR_BOX, AVATAR_BOX, GDK_INTERP_BILINEAR);

    if (scaled)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(ui->avatarImg), scaled);
        g_object_unref(scaled);
    }
    else
    {
        gtk_image_set_from_file(GTK_IMAGE(ui->avatarImg), ui->avatarFile.c_str());
    }

    gtk_widget_remove_css_class(ui->noAvatarText, "avatar-empty");
    gtk_frame_set_child(GTK_FRAME(ui->avatarFrame), ui->avatarImg);

    g_object_unref(pix);
    g_free(path);
}


// login button callback
void AppUI::tryLogin(GtkButton *button, gpointer user_data)
{
    (void)button;

    AppUI *ui = static_cast<AppUI*>(user_data);

    const char *u = gtk_editable_get_text(GTK_EDITABLE(ui->userIn));
    const char *p = gtk_editable_get_text(GTK_EDITABLE(ui->passIn));

    std::string user = u ? u : "";
    std::string pass = p ? p : "";

    if (checkLogin(user, pass))
    {
        ui->setLabel(ui->loginMsg, "Login successful.");
        gtk_stack_set_visible_child(GTK_STACK(ui->stack), ui->mainBox);
    }
    else
    {
        ui->setLabel(ui->loginMsg, "Invalid username or password.");
    }
}


// test button callback
void AppUI::testClick(GtkButton *button, gpointer user_data)
{
    (void)button;
    (void)user_data;
    g_print("Button was clicked!\n");
}


// show pie chart
void AppUI::showPie(GtkButton *button, gpointer user_data)
{
    (void)button;
    AppUI *ui = static_cast<AppUI*>(user_data);

    if (!ui->hasClass)
    {
        ui->setLabel(ui->fileText, "Load a scores file first.");
        return;
    }

    // drawing area
    GtkWidget *area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(area, TRUE);
    gtk_widget_set_vexpand(area, TRUE);

    gtk_drawing_area_set_draw_func(
        GTK_DRAWING_AREA(area),
        drawPieChart,
        &ui->cls,
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


// show bar chart
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

    // IMPORTANT: pass BarCtx*, not &ui->cls
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


// builds the login screen (UNCHANGED)
GtkWidget* AppUI::makeLogin(GtkWidget *win)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_hexpand(page, TRUE);
    gtk_widget_set_vexpand(page, TRUE);

    GtkWidget *left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(left, TRUE);
    gtk_widget_set_vexpand(left, TRUE);
    gtk_widget_add_css_class(left, "login-left");

    GtkWidget *app_title = gtk_label_new("Grade Calculator");
    gtk_widget_set_halign(app_title, GTK_ALIGN_START);
    gtk_widget_add_css_class(app_title, "app-title");
    gtk_box_append(GTK_BOX(left), app_title);

    GtkWidget *app_sub = gtk_label_new("Created by: Amara Whitson and Ben Yodena");
    gtk_widget_set_halign(app_sub, GTK_ALIGN_START);
    gtk_widget_add_css_class(app_sub, "app-subtitle");
    gtk_box_append(GTK_BOX(left), app_sub);

    GtkWidget *spacer = gtk_label_new("");
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(left), spacer);

    GtkWidget *right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(right, TRUE);
    gtk_widget_set_vexpand(right, TRUE);
    gtk_widget_add_css_class(right, "login-right");

    GtkWidget *title = gtk_label_new("Welcome Back!");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_add_css_class(title, "login-title");
    gtk_box_append(GTK_BOX(right), title);

    GtkWidget *user_label = gtk_label_new("Username:");
    gtk_widget_set_halign(user_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right), user_label);

    userIn = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(userIn), "Enter username");
    gtk_box_append(GTK_BOX(right), userIn);

    GtkWidget *pass_label = gtk_label_new("Password:");
    gtk_widget_set_halign(pass_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right), pass_label);

    passIn = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(passIn), "Enter password");
    gtk_entry_set_visibility(GTK_ENTRY(passIn), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(passIn), '*');
    gtk_box_append(GTK_BOX(right), passIn);

    loginMsg = gtk_label_new("");
    gtk_widget_set_halign(loginMsg, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right), loginMsg);

    GtkWidget *login_btn = gtk_button_new_with_label("Login");
    gtk_widget_add_css_class(login_btn, "login-btn");
    gtk_box_append(GTK_BOX(right), login_btn);

    g_signal_connect(login_btn, "clicked", G_CALLBACK(AppUI::tryLogin), this);

    gtk_window_set_default_widget(GTK_WINDOW(win), login_btn);

    gtk_box_append(GTK_BOX(page), left);
    gtk_box_append(GTK_BOX(page), right);

    return page;
}


// builds the main screen (same layout; connects Bar/Pie buttons properly)
GtkWidget* AppUI::makeMain(GtkWidget *win)
{
    mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_top(mainBox, 20);
    gtk_widget_set_margin_bottom(mainBox, 20);
    gtk_widget_set_margin_start(mainBox, 20);
    gtk_widget_set_margin_end(mainBox, 20);

    GtkWidget *sidebar_wrap = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(sidebar_wrap, TRUE);
    gtk_widget_add_css_class(sidebar_wrap, "sidebar");

    GtkWidget *top_spacer = gtk_label_new("");
    gtk_widget_set_vexpand(top_spacer, TRUE);

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *bot_spacer = gtk_label_new("");
    gtk_widget_set_vexpand(bot_spacer, TRUE);

    gtk_box_append(GTK_BOX(sidebar_wrap), top_spacer);
    gtk_box_append(GTK_BOX(sidebar_wrap), sidebar);
    gtk_box_append(GTK_BOX(sidebar_wrap), bot_spacer);

    GtkWidget *choosebtn = gtk_button_new_with_label("Choose File");
    g_signal_connect(choosebtn, "clicked", G_CALLBACK(AppUI::pickFile), this);
    gtk_box_append(GTK_BOX(sidebar), choosebtn);

    GtkWidget *avatarbtn = gtk_button_new_with_label("Upload Avatar");
    g_signal_connect(avatarbtn, "clicked", G_CALLBACK(AppUI::pickAvatar), this);
    gtk_box_append(GTK_BOX(sidebar), avatarbtn);

    gtk_box_append(GTK_BOX(sidebar), gtk_button_new_with_label("Sorting"));

    GtkWidget *barBtn = gtk_button_new_with_label("Bar Chart");
    g_signal_connect(barBtn, "clicked", G_CALLBACK(AppUI::showBar), this);
    gtk_box_append(GTK_BOX(sidebar), barBtn);

    GtkWidget *pieBtn = gtk_button_new_with_label("Pie Chart");
    g_signal_connect(pieBtn, "clicked", G_CALLBACK(AppUI::showPie), this);
    gtk_box_append(GTK_BOX(sidebar), pieBtn);

    GtkWidget *testbtn = gtk_button_new_with_label("Click Me");
    g_signal_connect(testbtn, "clicked", G_CALLBACK(AppUI::testClick), NULL);
    gtk_box_append(GTK_BOX(sidebar), testbtn);

    gtk_window_set_default_widget(GTK_WINDOW(win), choosebtn);

    gtk_box_append(GTK_BOX(mainBox), sidebar_wrap);

    // RIGHT CONTENT
    rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_add_css_class(rightBox, "content");

    GtkWidget *toprow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(rightBox), toprow);

    fileText = gtk_label_new("Selected: (none)");
    gtk_widget_set_hexpand(fileText, TRUE);
    gtk_widget_set_halign(fileText, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(toprow), fileText);

    avatarFrame = gtk_frame_new(NULL);
    gtk_widget_set_overflow(avatarFrame, GTK_OVERFLOW_HIDDEN);
    gtk_widget_add_css_class(avatarFrame, "avatar-frame");
    gtk_widget_set_size_request(avatarFrame, AVATAR_BOX, AVATAR_BOX);

    noAvatarText = gtk_label_new("no avatar");
    gtk_widget_add_css_class(noAvatarText, "avatar-empty");
    gtk_frame_set_child(GTK_FRAME(avatarFrame), noAvatarText);

    avatarImg = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(avatarImg), AVATAR_BOX);

    gtk_box_append(GTK_BOX(toprow), avatarFrame);

    textBox = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textBox), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textBox), FALSE);

    textScroll = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(textScroll, TRUE);
    gtk_widget_set_hexpand(textScroll, TRUE);
    gtk_widget_add_css_class(textScroll, "display-area");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(textScroll), textBox);

    gtk_box_append(GTK_BOX(rightBox), textScroll);
    centerWidget = textScroll;


    gtk_box_append(GTK_BOX(mainBox), rightBox);

    return mainBox;
}


// builds the whole window + stack pages (UNCHANGED)
void AppUI::run(GtkApplication *app)
{
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grade Calculator Practice");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, THEME, -1);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    stack = GTK_STACK(gtk_stack_new());
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(stack));

    loginBox = makeLogin(window);
    mainBox  = makeMain(window);

    gtk_stack_add_named(GTK_STACK(stack), loginBox, "login");
    gtk_stack_add_named(GTK_STACK(stack), mainBox,  "main");
    gtk_stack_set_visible_child(GTK_STACK(stack), loginBox);

    gtk_widget_show(window);
}
