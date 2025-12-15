// uilayout.cpp
// Builds the login and main UI layouts

#include "ui.h"


// builds the login screen
GtkWidget* AppUI::makeLogin(GtkWidget *win)
{
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_hexpand(page, TRUE);
    gtk_widget_set_vexpand(page, TRUE);

    // left side (title / credits)
    GtkWidget *left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(left, TRUE);
    gtk_widget_set_vexpand(left, TRUE);
    gtk_widget_add_css_class(left, "login-left");

    GtkWidget *appTitle = gtk_label_new("Grade Calculator");
    gtk_widget_set_halign(appTitle, GTK_ALIGN_START);
    gtk_widget_add_css_class(appTitle, "app-title");
    gtk_box_append(GTK_BOX(left), appTitle);

    GtkWidget *subtitle =
        gtk_label_new("Created by: Amara Whitson and Ben Yodena");
    gtk_widget_set_halign(subtitle, GTK_ALIGN_START);
    gtk_widget_add_css_class(subtitle, "app-subtitle");
    gtk_box_append(GTK_BOX(left), subtitle);

    // spacer pushes content toward the top
    GtkWidget *spacer = gtk_label_new("");
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(left), spacer);

    // right side (login form)
    GtkWidget *right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(right, TRUE);
    gtk_widget_set_vexpand(right, TRUE);
    gtk_widget_add_css_class(right, "login-right");

    GtkWidget *title = gtk_label_new("Welcome Back!");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_add_css_class(title, "login-title");
    gtk_box_append(GTK_BOX(right), title);

    GtkWidget *userLabel = gtk_label_new("Username:");
    gtk_widget_set_halign(userLabel, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right), userLabel);

    userIn = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(userIn), "Enter username");
    gtk_box_append(GTK_BOX(right), userIn);

    GtkWidget *passLabel = gtk_label_new("Password:");
    gtk_widget_set_halign(passLabel, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right), passLabel);

    passIn = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(passIn), "Enter password");
    gtk_entry_set_visibility(GTK_ENTRY(passIn), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(passIn), '*');
    gtk_box_append(GTK_BOX(right), passIn);

    loginMsg = gtk_label_new("");
    gtk_widget_set_halign(loginMsg, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right), loginMsg);

    GtkWidget *loginBtn = gtk_button_new_with_label("Login");
    gtk_widget_add_css_class(loginBtn, "login-btn");
    gtk_box_append(GTK_BOX(right), loginBtn);

    g_signal_connect(loginBtn, "clicked",
                     G_CALLBACK(AppUI::tryLogin), this);

    gtk_window_set_default_widget(GTK_WINDOW(win), loginBtn);

    gtk_box_append(GTK_BOX(page), left);
    gtk_box_append(GTK_BOX(page), right);

    return page;
}


// builds the main application screen
GtkWidget* AppUI::makeMain(GtkWidget *win)
{
    (void)win;

    mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_margin_top(mainBox, 20);
    gtk_widget_set_margin_bottom(mainBox, 20);
    gtk_widget_set_margin_start(mainBox, 20);
    gtk_widget_set_margin_end(mainBox, 20);

    // left sidebar (buttons)
    GtkWidget *sidebarWrap = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(sidebarWrap, TRUE);
    gtk_widget_add_css_class(sidebarWrap, "sidebar");

    GtkWidget *topSpacer = gtk_label_new("");
    gtk_widget_set_vexpand(topSpacer, TRUE);

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *botSpacer = gtk_label_new("");
    gtk_widget_set_vexpand(botSpacer, TRUE);

    gtk_box_append(GTK_BOX(sidebarWrap), topSpacer);
    gtk_box_append(GTK_BOX(sidebarWrap), sidebar);
    gtk_box_append(GTK_BOX(sidebarWrap), botSpacer);

    GtkWidget *chooseBtn = gtk_button_new_with_label("Choose File");
    g_signal_connect(chooseBtn, "clicked",
                     G_CALLBACK(AppUI::pickFile), this);
    gtk_box_append(GTK_BOX(sidebar), chooseBtn);

    GtkWidget *avatarBtn = gtk_button_new_with_label("Upload Avatar");
    g_signal_connect(avatarBtn, "clicked",
                     G_CALLBACK(AppUI::pickAvatar), this);
    gtk_box_append(GTK_BOX(sidebar), avatarBtn);

    GtkWidget *sortBtn = gtk_button_new_with_label("Sorting");
    g_signal_connect(sortBtn, "clicked",
                     G_CALLBACK(AppUI::sortStudents), this);
    gtk_box_append(GTK_BOX(sidebar), sortBtn);

    GtkWidget *barBtn = gtk_button_new_with_label("Bar Chart");
    g_signal_connect(barBtn, "clicked",
                     G_CALLBACK(AppUI::showBar), this);
    gtk_box_append(GTK_BOX(sidebar), barBtn);

    GtkWidget *pieBtn = gtk_button_new_with_label("Pie Chart");
    g_signal_connect(pieBtn, "clicked",
                     G_CALLBACK(AppUI::showPie), this);
    gtk_box_append(GTK_BOX(sidebar), pieBtn);

    gtk_box_append(GTK_BOX(mainBox), sidebarWrap);

    // right side (top info + center display)
    rightBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_add_css_class(rightBox, "content");

    GtkWidget *topRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(rightBox), topRow);

    fileText = gtk_label_new("Selected: (none)");
    gtk_widget_set_hexpand(fileText, TRUE);
    gtk_widget_set_halign(fileText, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(topRow), fileText);

    // avatar area
    avatarFrame = gtk_frame_new(NULL);
    gtk_widget_set_overflow(avatarFrame, GTK_OVERFLOW_HIDDEN);
    gtk_widget_add_css_class(avatarFrame, "avatar-frame");
    gtk_widget_set_size_request(avatarFrame, AVATAR_BOX, AVATAR_BOX);

    noAvatarText = gtk_label_new("no avatar");
    gtk_widget_add_css_class(noAvatarText, "avatar-empty");
    gtk_frame_set_child(GTK_FRAME(avatarFrame), noAvatarText);

    avatarImg = gtk_image_new();
    gtk_image_set_pixel_size(GTK_IMAGE(avatarImg), AVATAR_BOX);

    gtk_box_append(GTK_BOX(topRow), avatarFrame);

    // text display area (default center view)
    textBox = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textBox), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textBox), FALSE);

    textScroll = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(textScroll, TRUE);
    gtk_widget_set_vexpand(textScroll, TRUE);
    gtk_widget_add_css_class(textScroll, "display-area");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(textScroll), textBox);

    // keep widgets alive even when swapped out
    g_object_ref(textBox);
    g_object_ref(textScroll);

    gtk_box_append(GTK_BOX(rightBox), textScroll);
    centerWidget = textScroll;

    gtk_box_append(GTK_BOX(mainBox), rightBox);

    return mainBox;
}
