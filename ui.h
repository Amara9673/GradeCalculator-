#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include <string>

#include "chart.h"
#include "myClass.h"

// AppUI holds the GTK widgets + the class data so callbacks can access everything.
class AppUI
{
public:
    AppUI();
    void run(GtkApplication *app);

private:
    // small UI constants
    static const int AVATAR_BOX = 64;  // avatar image size in pixels

    // keeps track of how the student list is currently sorted
    int sortMode;

    // chart context (used by the bar chart code)
    BarCtx barCtx;

    // top-level widgets
    GtkWidget *window;
    GtkStack  *stack;

    // main pages in the stack
    GtkWidget *loginBox;
    GtkWidget *mainBox;

    // login page widgets
    GtkWidget *userIn;
    GtkWidget *passIn;
    GtkWidget *loginMsg;

    // avatar widgets (
    GtkWidget *avatarFrame;
    GtkWidget *avatarImg;
    GtkWidget *noAvatarText;
    std::string avatarFile;

    // file UI widgets
    GtkWidget *textBox;     // the big text view in the center
    GtkWidget *fileText;    // label showing current loaded file
    std::string curFile;

    // center swapping (text view vs charts)
    GtkWidget *rightBox;        // holds the top row + the center content
    GtkWidget *centerWidget;    // whichever widget is currently in the center
    GtkWidget *textScroll;      // scrolled window that always holds textBox

    // class data loaded from file
    myClass cls;
    bool hasClass;

    // helper functions 
    void setLabel(GtkWidget *label, const char *msg);
    void setText(const std::string& text);
    void swapCenter(GtkWidget *newCenter);
    std::string buildStudentSummary();

    // page builders
    GtkWidget* makeLogin(GtkWidget *win);
    GtkWidget* makeMain(GtkWidget *win);

    // callbacks (static because GTK uses C-style function pointers)
    static void tryLogin(GtkButton *button, gpointer user_data);
    static void sortStudents(GtkButton *button, gpointer user_data);

    static void pickFile(GtkButton *btn, gpointer data);
    static void pickedFile(GObject *source, GAsyncResult *res, gpointer data);

    static void pickAvatar(GtkButton *btn, gpointer data);
    static void pickedAvatar(GObject *source, GAsyncResult *res, gpointer data);

    static void showBar(GtkButton *button, gpointer user_data);
    static void showPie(GtkButton *button, gpointer user_data);
};

#endif
