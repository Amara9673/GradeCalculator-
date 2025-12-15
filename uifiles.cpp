// uifiles.cpp
// Handles choosing the scores file and uploading an avatar

#include "ui.h"

#include <fstream>
#include <string>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "load.h"


// opens the file picker for the scores file
void AppUI::pickFile(GtkButton *btn, gpointer data)
{
    (void)btn;
    AppUI *ui = static_cast<AppUI*>(data);

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Choose a scores file");

    // basic filter to help the user choose .txt files
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
    if (file == NULL)
        return; // user cancelled

    char *path = g_file_get_path(file);
    g_object_unref(file);

    if (path == NULL)
        return;

    ui->curFile = path;
    g_free(path);

    // quick check: only allow .txt
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

    // show selected file path
    std::string labelText = "Selected: " + ui->curFile;
    ui->setLabel(ui->fileText, labelText.c_str());

    // read raw file text so it can be displayed
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

    // try to parse into class object
    myClass temp;
    if (loadScoresFile(ui->curFile, temp))
    {
        ui->cls = temp;
        ui->hasClass = true;

        ui->barCtx.cls = &ui->cls;
        ui->barCtx.studentIndex = 0;
    }
    else
    {
        ui->hasClass = false;
        ui->barCtx.cls = NULL;
        ui->barCtx.studentIndex = 0;
    }

    // go back to text view as default after picking a file
    if (ui->textScroll != NULL)
        ui->swapCenter(ui->textScroll);

    ui->setText(contents);
}


// opens the file picker for the avatar image
void AppUI::pickAvatar(GtkButton *btn, gpointer data)
{
    (void)btn;
    AppUI *ui = static_cast<AppUI*>(data);

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Choose an avatar image");

    gtk_file_dialog_open(dialog, GTK_WINDOW(ui->window), NULL, AppUI::pickedAvatar, ui);
    g_object_unref(dialog);
}


// loads the avatar image and puts it into the avatar frame
void AppUI::pickedAvatar(GObject *source, GAsyncResult *res, gpointer data)
{
    AppUI *ui = static_cast<AppUI*>(data);

    GFile *file = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(source), res, NULL);
    if (file == NULL)
        return; // user cancelled

    char *path = g_file_get_path(file);
    g_object_unref(file);

    if (path == NULL)
        return;

    ui->avatarFile = path;

    // load image
    GError *err = NULL;
    GdkPixbuf *pix = gdk_pixbuf_new_from_file(path, &err);

    if (!pix)
    {
        ui->setLabel(ui->noAvatarText, "could not load image");

        if (err)
            g_error_free(err);

        g_free(path);
        return;
    }

    // scale into a square
    GdkPixbuf *scaled =
        gdk_pixbuf_scale_simple(pix, AVATAR_BOX, AVATAR_BOX, GDK_INTERP_BILINEAR);

    if (scaled)
    {
        gtk_image_set_from_pixbuf(GTK_IMAGE(ui->avatarImg), scaled);
        g_object_unref(scaled);
    }
    else
    {
        // fallback if scaling fails
        gtk_image_set_from_file(GTK_IMAGE(ui->avatarImg), ui->avatarFile.c_str());
    }

    // remove the "empty" styling and show the image
    gtk_widget_remove_css_class(ui->noAvatarText, "avatar-empty");
    gtk_frame_set_child(GTK_FRAME(ui->avatarFrame), ui->avatarImg);

    g_object_unref(pix);
    g_free(path);
}
