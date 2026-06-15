#include <adwaita.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

static GtkWidget *window_global;

static void on_sre_clicked(GtkButton *btn, gpointer user_data) {
    g_print("SRE selected (placeholder start)\n");
    gtk_window_close(GTK_WINDOW(window_global));
}

static GtkWidget* create_session_card(const char *id, const char *title, const char *meta) {
    GtkWidget *card = gtk_button_new();
    gtk_widget_set_size_request(card, 160, 70);
    gtk_widget_add_css_class(card, "card");

    gtk_widget_set_opacity(card, 0.85);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);

    gtk_widget_set_margin_top(box, 10);
    gtk_widget_set_margin_bottom(box, 10);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);

    GtkWidget *i = gtk_label_new(id);
    gtk_widget_set_halign(i, GTK_ALIGN_START);
    gtk_widget_add_css_class(i, "caption-heading");

    GtkWidget *t = gtk_label_new(title);
    gtk_widget_set_halign(i, GTK_ALIGN_START);
    gtk_widget_add_css_class(i, "caption-heading");

    GtkWidget *d = gtk_label_new(meta);
    gtk_widget_set_halign(d, GTK_ALIGN_START);
    gtk_widget_add_css_class(d, "dim-label");

    gtk_label_set_ellipsize(GTK_LABEL(d), PANGO_ELLIPSIZE_END);

    gtk_box_append(GTK_BOX(box), i);
    gtk_box_append(GTK_BOX(box), t);
    gtk_box_append(GTK_BOX(box), d);

    gtk_button_set_child(GTK_BUTTON(card), box);

    return card;
}

static GtkWidget* create_project_card(const char *title, const char *desc, gboolean enabled, GCallback callback) {
    GtkWidget *card = gtk_button_new();
    gtk_widget_set_size_request(card, 260, 140);
    gtk_widget_add_css_class(card, "card");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);

    GtkWidget *t = gtk_label_new(title);
    gtk_widget_set_halign(t, GTK_ALIGN_START);
    gtk_widget_add_css_class(t, "title-3");

    GtkWidget *d = gtk_label_new(desc);
    gtk_widget_set_halign(d, GTK_ALIGN_START);
    gtk_widget_add_css_class(d, "dim-label");

    gtk_label_set_wrap(GTK_LABEL(d), TRUE);
    gtk_label_set_wrap_mode(GTK_LABEL(d), PANGO_WRAP_WORD_CHAR);

    gtk_box_append(GTK_BOX(box), t);
    gtk_box_append(GTK_BOX(box), d);

    gtk_button_set_child(GTK_BUTTON(card), box);
    gtk_widget_set_sensitive(card, enabled);

    if (enabled && callback) g_signal_connect(card, "clicked", callback, NULL);
    return card;
}


static void on_activate(GtkApplication *app) {
    window_global = adw_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window_global), 1000, 500);

    GtkWidget *root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(root_box, TRUE);
    gtk_widget_set_hexpand(root_box, TRUE);

    GtkWidget *header = adw_header_bar_new();
    GtkWidget *title = adw_window_title_new("AV Toolkit", "");
    adw_header_bar_set_title_widget(ADW_HEADER_BAR(header), title);

    GtkWidget *landing = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(landing, TRUE);
    gtk_widget_set_hexpand(landing, TRUE);
    gtk_widget_set_valign(landing, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(landing, GTK_ALIGN_CENTER);



    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_halign(content, GTK_ALIGN_CENTER);

    GtkWidget *app_title = gtk_label_new("AV Toolkit");
    gtk_widget_add_css_class(app_title, "title-1");
    GtkWidget *subtitle = gtk_label_new("Choose a project type to begin");
    gtk_widget_add_css_class(subtitle, "dim-label");
    gtk_box_append(GTK_BOX(content), app_title);
    gtk_box_append(GTK_BOX(content), subtitle);


    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 14);
    gtk_widget_set_halign(row, GTK_ALIGN_CENTER);

    GtkWidget *sre = create_project_card(
        "SRE", "Shorts Reverse Engineering:\n\nSteal viral video structures from YouTube Shorts",
        TRUE, G_CALLBACK(on_sre_clicked)
    );
    GtkWidget *abc = create_project_card("ABC", "Coming soon\n\nNot implemented yet", FALSE, NULL);
    GtkWidget *def = create_project_card("DEF", "Coming soon\n\nNot implemented yet", FALSE, NULL);

    gtk_box_append(GTK_BOX(row), sre);
    gtk_box_append(GTK_BOX(row), abc);
    gtk_box_append(GTK_BOX(row), def);
    gtk_box_append(GTK_BOX(content), row);



    GtkWidget *recent_label = gtk_label_new("Recent sessions");
    gtk_widget_add_css_class(recent_label, "dim-label");
    gtk_widget_set_halign(recent_label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(recent_label, 6);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);

    GtkWidget *session_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    gtk_box_append(GTK_BOX(session_row),
        create_session_card("SRE #1001", "Captain America: Civil War Edit", "41 minutes ago"));
    gtk_box_append(GTK_BOX(session_row),
        create_session_card("SRE #1042", "The Rookie: Season 4 Opening", "2 hours ago"));
    gtk_box_append(GTK_BOX(session_row),
        create_session_card("SRE #1039", "James Bond: Skyfall Edit", "Yesterday"));
    gtk_box_append(GTK_BOX(session_row),
        create_session_card("SRE #1032", "The Rookie: Dim and Juicy", "3 days ago"));
    gtk_box_append(GTK_BOX(session_row),
        create_session_card("SRE #1028", "The Day of the Jackal Edit", "Last week"));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), session_row);

    gtk_box_append(GTK_BOX(content), recent_label);
    gtk_box_append(GTK_BOX(content), scroll);



    AdwClamp *clamp = ADW_CLAMP(adw_clamp_new());
    adw_clamp_set_maximum_size(clamp, 700);
    adw_clamp_set_child(clamp, content);
    gtk_box_append(GTK_BOX(landing), GTK_WIDGET(clamp));

    gtk_box_append(GTK_BOX(root_box), header);
    gtk_box_append(GTK_BOX(root_box), landing);

    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window_global), root_box);
    gtk_window_present(GTK_WINDOW(window_global));
}


int main(int argc, char *argv[]) {
    adw_init();
    AdwApplication *app = adw_application_new("net.ranoe.avtoolkit", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}