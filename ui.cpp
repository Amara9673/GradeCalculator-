// ui.cpp
// Main UI file (core helpers + summary + login/sort callbacks + app startup)

#include "ui.h"

#include <sstream>
#include <iomanip>
#include <string>

#include "authentication.h"
#include "theme.h"


AppUI::AppUI()
{
    // app starts with no loaded class
    hasClass = false;

    // 0 = id, 1 = letter, 2 = overall %
    sortMode = 0;

    // bar chart context (only valid after loading a class)
    barCtx.cls = NULL;
    barCtx.studentIndex = 0;

    // window / pages
    window = NULL;
    stack = NULL;

    loginBox = NULL;
    mainBox = NULL;

    // login widgets
    userIn = NULL;
    passIn = NULL;
    loginMsg = NULL;

    // avatar widgets
    avatarFrame = NULL;
    avatarImg = NULL;
    noAvatarText = NULL;
    avatarFile = "";

    // file display widgets
    textBox = NULL;
    fileText = NULL;
    curFile = "";

    // swapping the center area (text view vs charts)
    rightBox = NULL;
    centerWidget = NULL;
    textScroll = NULL;
}


// safely set label text (avoids crashing if something isn't built yet)
void AppUI::setLabel(GtkWidget *label, const char *msg)
{
    if (label != NULL)
        gtk_label_set_text(GTK_LABEL(label), msg);
}


// writes a string into the text view in the center
// GTK text buffers expect UTF-8, so we validate first
void AppUI::setText(const std::string& text)
{
    if (textBox == NULL)
        return;

    if (!GTK_IS_TEXT_VIEW(textBox))
        return;

    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textBox));
    if (buf == NULL)
        return;

    if (g_utf8_validate(text.c_str(), -1, NULL))
        gtk_text_buffer_set_text(buf, text.c_str(), -1);
    else
        gtk_text_buffer_set_text(buf, "could not display text\n", -1);
}


// swaps what is shown in the center (text scroll vs chart frame)
void AppUI::swapCenter(GtkWidget *newCenter)
{
    if (rightBox == NULL || newCenter == NULL)
        return;

    // remove old center widget if it is different
    if (centerWidget != NULL && centerWidget != newCenter)
        gtk_box_remove(GTK_BOX(rightBox), centerWidget);

    // don't append a widget twice
    if (gtk_widget_get_parent(newCenter) == NULL)
        gtk_box_append(GTK_BOX(rightBox), newCenter);

    centerWidget = newCenter;
}


// builds a readable summary of students + class averages (for the text view)
std::string AppUI::buildStudentSummary()
{
    if (!hasClass)
        return "Load a scores file first.\n";

    std::ostringstream out;

    out << "Students: " << cls.getStudentsLength() << "\n";
    out << std::fixed << std::setprecision(1);

    out << "Class Averages (percent):\n";
    out << "  Labs:    " << cls.getClassLabScore() << "\n";
    out << "  Quizzes: " << cls.getClassQuizScore() << "\n";
    out << "  Midterm: " << cls.getClassMidtermScore() << "\n";
    out << "  Project: " << cls.getClassProjectScore() << "\n";
    out << "  Final:   " << cls.getClassFinalScore() << "\n\n";

    out << "ID        Letter  Overall%   Labs%   Quiz%   Mid%   Proj%  Final%\n";
    out << "------------------------------------------------------------------\n";

    for (int i = 0; i < cls.getStudentsLength(); i++)
    {
        int id = cls.getStudent(i).getID();
        std::string letter = cls.getLetter(i);
        double overall = cls.getTotalScore(i);

        out << std::setw(9) << std::setfill('0') << id << std::setfill(' ')
            << "   " << std::setw(2) << letter
            << "     " << std::setw(7) << overall
            << "   " << std::setw(6) << cls.getLabScore(i)
            << " "  << std::setw(6) << cls.getQuizScore(i)
            << " "  << std::setw(6) << cls.getMidtermScore(i)
            << " "  << std::setw(6) << cls.getProjectScore(i)
            << " "  << std::setw(6) << cls.getFinalScore(i)
            << "\n";
    }

    return out.str();
}


// login button callback
void AppUI::tryLogin(GtkButton *button, gpointer user_data)
{
    (void)button;
    AppUI *ui = static_cast<AppUI*>(user_data);

    const char *u = gtk_editable_get_text(GTK_EDITABLE(ui->userIn));
    const char *p = gtk_editable_get_text(GTK_EDITABLE(ui->passIn));

    std::string user = (u ? u : "");
    std::string pass = (p ? p : "");

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


// cycles sorting mode and shows the student summary
void AppUI::sortStudents(GtkButton *button, gpointer user_data)
{
    (void)button;
    AppUI *ui = static_cast<AppUI*>(user_data);

    if (!ui->hasClass)
    {
        ui->setLabel(ui->fileText, "Load a scores file first.");
        return;
    }

    // cycle: 0 -> 1 -> 2 -> 0 ...
    ui->sortMode = (ui->sortMode + 1) % 3;

    if (ui->sortMode == 0)
    {
        ui->cls.sortID();
        ui->setLabel(ui->fileText, "Sorting by ID");
    }
    else if (ui->sortMode == 1)
    {
        ui->cls.sortLetter();
        ui->setLabel(ui->fileText, "Sorting by Letter Grade");
    }
    else
    {
        ui->cls.sortGrade();
        ui->setLabel(ui->fileText, "Sorting by Overall Percentage");
    }

    // make sure the scroll text view is visible
    if (ui->textScroll != NULL)
        ui->swapCenter(ui->textScroll);

    ui->setText(ui->buildStudentSummary());
}


// creates the window and loads both pages into the stack
void AppUI::run(GtkApplication *app)
{
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Grade Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // load CSS theme from theme.h
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, THEME, -1);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    // stack holds login page + main page
    stack = GTK_STACK(gtk_stack_new());
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(stack));

    loginBox = makeLogin(window);
    mainBox = makeMain(window);

    gtk_stack_add_named(GTK_STACK(stack), loginBox, "login");
    gtk_stack_add_named(GTK_STACK(stack), mainBox, "main");
    gtk_stack_set_visible_child(GTK_STACK(stack), loginBox);

    gtk_widget_show(window);
}
