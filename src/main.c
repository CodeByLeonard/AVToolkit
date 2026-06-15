#include <adwaita.h>
#include <gtk/gtk.h>
#include <pango/pango.h>
#include "main.h"

#include <sqlite3.h>

#include "database.h"
#include "pages/welcome_page.h"

static GtkWidget *window_global;
static GtkWidget *page_container;

void switch_page(GtkWidget *new_page) {
    GtkWidget *child = gtk_widget_get_first_child(page_container);

    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(page_container), child);
        child = next;
    }

    gtk_box_append(GTK_BOX(page_container), new_page);
}

static void on_home_clicked(GtkButton *btn, gpointer user_data) {
    switch_page(build_welcome_page());
}

static void on_activate(GtkApplication *app) {
    window_global = adw_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window_global), 1000, 500);

    GtkWidget *root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *header = adw_header_bar_new();
    GtkWidget *title = adw_window_title_new("AV Toolkit", "");
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(header), title);

    GtkWidget *home_btn = gtk_button_new_with_label("↺");
    gtk_widget_add_css_class(home_btn, "flat");
    g_signal_connect(home_btn, "clicked", G_CALLBACK(on_home_clicked), NULL);

    adw_header_bar_pack_start(ADW_HEADER_BAR(header), home_btn);

    page_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(page_container), build_welcome_page());

    gtk_box_append(GTK_BOX(root_box), header);
    gtk_box_append(GTK_BOX(root_box), page_container);

    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window_global), root_box);
    gtk_window_present(GTK_WINDOW(window_global));
}


int main(int argc, char *argv[]) {
    sqlite3 *db = db_open();
    db_init(db);

    adw_init();
    adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_PREFER_DARK);
    AdwApplication *app = adw_application_new("net.ranoe.avtoolkit", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}