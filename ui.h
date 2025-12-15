#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <string>
#include "myClass.h"


class AppUI {
public:
    AppUI();

    // runs the whole app window
    void run(GtkApplication *app);

private:
    myClass cls;
    bool hasClass;

    // screens
    GtkStack  *stack;
    GtkWidget *loginBox;
    GtkWidget *mainBox;

    // window
    GtkWidget *window;

    // login widgets
    GtkWidget *userIn;
    GtkWidget *passIn;
    GtkWidget *loginMsg;

    // avatar widgets
    GtkWidget *avatarFrame;
    GtkWidget *avatarImg;
    GtkWidget *noAvatarText;
    std::string avatarFile;
    static const int AVATAR_BOX = 120;

    // file display widgets
    GtkWidget *textBox;
    GtkWidget *fileText;
    std::string curFile;

    // small helpers
    void setText(const std::string& text);
    void setLabel(GtkWidget *label, const char *msg);

    // file picker callbacks
    static void pickFile(GtkButton *btn, gpointer data);
    static void pickedFile(GObject *source, GAsyncResult *res, gpointer data);

    // avatar picker callbacks
    static void pickAvatar(GtkButton *btn, gpointer data);
    static void pickedAvatar(GObject *source, GAsyncResult *res, gpointer data);

    // button callbacks
    static void tryLogin(GtkButton *button, gpointer user_data);
    static void testClick(GtkButton *button, gpointer user_data);

    // build each screen
    GtkWidget* makeLogin(GtkWidget *window);
    GtkWidget* makeMain(GtkWidget *window);
};

#endif
