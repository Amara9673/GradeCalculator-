#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <string>

#include "myClass.h"

// avatar size (matches your UI usage)
const int AVATAR_BOX = 64;

class AppUI
{
public:
    AppUI();
    void run(GtkApplication *app);

private:
    // ---- core widgets ----
    GtkWidget *window;
    GtkStack  *stack;

    GtkWidget *loginBox;
    GtkWidget *mainBox;

    // ---- login widgets ----
    GtkWidget *userIn;
    GtkWidget *passIn;
    GtkWidget *loginMsg;

    // ---- avatar widgets ----
    GtkWidget *avatarFrame;
    GtkWidget *avatarImg;
    GtkWidget *noAvatarText;
    std::string avatarFile;

    // ---- file display widgets ----
    GtkWidget *textBox;
    GtkWidget *fileText;
    std::string curFile;

    // ---- center swap support ----
    GtkWidget *rightBox;       // container for top row + center content
    GtkWidget *centerWidget;   // current center widget (scroll/chart)

    // ---- class data ----
    myClass cls;
    bool hasClass;

    // ---- helpers ----
    void setLabel(GtkWidget *label, const char *msg);
    void setText(const std::string& text);
    void swapCenter(GtkWidget *newCenter);

    // ---- page builders ----
    GtkWidget* makeLogin(GtkWidget *win);
    GtkWidget* makeMain(GtkWidget *win);

    // ---- callbacks ----
    static void tryLogin(GtkButton *button, gpointer user_data);

    static void pickFile(GtkButton *btn, gpointer data);
    static void pickedFile(GObject *source, GAsyncResult *res, gpointer data);

    static void pickAvatar(GtkButton *btn, gpointer data);
    static void pickedAvatar(GObject *source, GAsyncResult *res, gpointer data);

    static void testClick(GtkButton *button, gpointer user_data);

    static void showBar(GtkButton *button, gpointer user_data);
    static void showPie(GtkButton *button, gpointer user_data);
};

#endif
