#include <stdio.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

static void set_big_title(GtkWidget *label, double scale) {
    PangoAttrList *attrs = pango_attr_list_new();
    PangoAttribute *s = pango_attr_scale_new(scale);
    pango_attr_list_insert(attrs, s);

    gtk_label_set_attributes(GTK_LABEL(label), attrs);
    pango_attr_list_unref(attrs);

    gtk_widget_set_halign(label, GTK_ALIGN_START);
}

static GtkWidget* create_segmented_control() {
    GtkWidget *bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_set_margin_top(bar, 10);
    gtk_widget_set_margin_bottom(bar, 10);

    GtkWidget *abc = gtk_toggle_button_new_with_label("ABC");
    GtkWidget *sre = gtk_toggle_button_new_with_label("SRE");
    GtkWidget *def = gtk_toggle_button_new_with_label("DEF");

    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(abc), NULL);
    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(sre), GTK_TOGGLE_BUTTON(abc));
    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(def), GTK_TOGGLE_BUTTON(abc));

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sre), TRUE);

    gtk_box_append(GTK_BOX(bar), abc);
    gtk_box_append(GTK_BOX(bar), sre);
    gtk_box_append(GTK_BOX(bar), def);

    return bar;
}

static GtkWidget* get_left() {
    GtkWidget *left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    gtk_widget_set_margin_top(left, 15);
    gtk_widget_set_margin_bottom(left, 15);
    gtk_widget_set_margin_start(left, 15);
    gtk_widget_set_margin_end(left, 15);

    GtkWidget *left_title = gtk_label_new("New Project");
    set_big_title(left_title, 1.4);

    GtkWidget *segment = create_segmented_control();

    GtkWidget *input_label = gtk_label_new("Input YouTube Shorts ID here:");
    gtk_widget_set_halign(input_label, GTK_ALIGN_START);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry),"e.g. dQw4w9WgXcQ");

    gtk_box_append(GTK_BOX(left), left_title);
    gtk_box_append(GTK_BOX(left), segment);
    gtk_box_append(GTK_BOX(left), input_label);
    gtk_box_append(GTK_BOX(left), entry);

    return left;
}

static GtkWidget* create_card(const char *title) {
    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    gtk_widget_set_margin_top(card, 8);
    gtk_widget_set_margin_bottom(card, 8);
    gtk_widget_set_margin_start(card, 8);
    gtk_widget_set_margin_end(card, 8);

    GtkWidget *label = gtk_label_new(title);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(card), label);

    return card;
}

static GtkWidget* get_right() {
    GtkWidget *right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);

    gtk_widget_set_margin_top(right, 15);
    gtk_widget_set_margin_bottom(right, 15);
    gtk_widget_set_margin_start(right, 15);
    gtk_widget_set_margin_end(right, 15);

    GtkWidget *right_title = gtk_label_new("Recent Projects");
    set_big_title(right_title, 1.4);

    gtk_box_append(GTK_BOX(right), right_title);
    gtk_box_append(GTK_BOX(right), create_card("Project Alpha"));
    gtk_box_append(GTK_BOX(right), create_card("SRE Pipeline Test"));
    gtk_box_append(GTK_BOX(right), create_card("DEF Placeholder Project"));

    return right;
}

static void on_activate(GtkApplication *app) {

    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "AV-Toolkit");
    gtk_window_set_default_size(GTK_WINDOW(window), 1100, 700);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_top(root, 20);
    gtk_widget_set_margin_bottom(root, 20);
    gtk_widget_set_margin_start(root, 20);
    gtk_widget_set_margin_end(root, 20);

    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    GtkWidget *welcome = gtk_label_new("Welcome back to AV-Toolkit!");
    set_big_title(welcome, 2.0);
    gtk_widget_set_halign(welcome, GTK_ALIGN_START);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(separator, 8);
    gtk_widget_set_margin_bottom(separator, 12);

    gtk_box_append(GTK_BOX(header_box), welcome);
    gtk_box_append(GTK_BOX(header_box), separator);

    GtkWidget *lower = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_vexpand(lower, TRUE);

    gtk_paned_set_start_child(GTK_PANED(lower), get_left());
    gtk_paned_set_end_child(GTK_PANED(lower), get_right());
    gtk_paned_set_position(GTK_PANED(lower), 550);

    gtk_box_append(GTK_BOX(root), header_box);
    gtk_box_append(GTK_BOX(root), lower);

    gtk_window_set_child(GTK_WINDOW(window), root);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("net.ranoe.avtoolkit", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}