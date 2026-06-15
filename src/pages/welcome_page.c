#include <gtk/gtk.h>
#include <adwaita.h>
#include "welcome_page.h"

#include "sre_page.h"
#include "../main.h"

static void on_create_session(GtkButton *btn, gpointer user_data) {
    AdwMessageDialog *dialog = ADW_MESSAGE_DIALOG(user_data);

    GtkWidget *entry = g_object_get_data(G_OBJECT(dialog), "entry");
    const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));

    g_print("Creating session: %s\n", text);

    gtk_window_destroy(GTK_WINDOW(dialog));

    switch_page(build_sre_page());
}

static void on_session_dialog_response(AdwMessageDialog *dialog,
                                       const char *response,
                                       gpointer user_data) {
    if (g_strcmp0(response, "create") == 0) {

        GtkWidget *entry = g_object_get_data(G_OBJECT(dialog), "entry");
        const char *text = gtk_editable_get_text(GTK_EDITABLE(entry));

        g_print("Creating session: %s\n", text);

        switch_page(build_sre_page());
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void on_sre_clicked(GtkButton *btn, gpointer user_data) {

    AdwMessageDialog *dialog =
        ADW_MESSAGE_DIALOG(adw_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(btn))),
            "Create Session",
            "Enter a session title before continuing"
        ));

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Session title...");

    adw_message_dialog_set_extra_child(dialog, entry);

    g_object_set_data(G_OBJECT(dialog), "entry", entry);

    adw_message_dialog_add_response(dialog, "cancel", "Cancel");
    adw_message_dialog_add_response(dialog, "create", "Create Session");

    adw_message_dialog_set_response_appearance(
        dialog,
        "create",
        ADW_RESPONSE_SUGGESTED
    );

    g_signal_connect(dialog, "response",
                     G_CALLBACK(on_session_dialog_response),
                     NULL);

    gtk_window_present(GTK_WINDOW(dialog));
}

static void on_recent_clicked(GtkButton *btn, gpointer user_data) {
    g_print("Clicked session: %s\n", (char *) user_data);
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

    GtkWidget *d = gtk_label_new(meta);
    gtk_widget_set_halign(d, GTK_ALIGN_START);
    gtk_widget_add_css_class(d, "dim-label");

    gtk_label_set_ellipsize(GTK_LABEL(d), PANGO_ELLIPSIZE_END);

    gtk_box_append(GTK_BOX(box), i);
    gtk_box_append(GTK_BOX(box), t);
    gtk_box_append(GTK_BOX(box), d);

    gtk_button_set_child(GTK_BUTTON(card), box);

    g_signal_connect(card, "clicked", G_CALLBACK(on_recent_clicked), (gpointer) id);

    return card;
}

static GtkWidget* create_project_card(const char *title, const char *desc, gboolean enabled) {
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

    if (enabled) g_signal_connect(card, "clicked", (GCallback) on_sre_clicked, NULL);
    return card;
}

GtkWidget* build_welcome_page(void) {
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

    GtkWidget *sre = create_project_card("SRE", "Shorts Reverse Engineering:\n\nSteal viral videos from the YouTube Shorts platform", TRUE);
    GtkWidget *abc = create_project_card("MSE (Coming soon)", "Movie Soundtrack Extracting:\n\nExtract gatekept sounds and music at maximum quality", FALSE);
    GtkWidget *def = create_project_card("CEA (Coming soon)", "Cinematic Edit Acquisition:\n\nRip epic character edits and their blueprint", FALSE);
    gtk_box_append(GTK_BOX(row), sre);
    gtk_box_append(GTK_BOX(row), abc);
    gtk_box_append(GTK_BOX(row), def);

    gtk_box_append(GTK_BOX(content), row);


    GtkWidget *recent_label = gtk_label_new("Recent sessions");
    gtk_widget_add_css_class(recent_label, "dim-label");
    gtk_widget_set_halign(recent_label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(recent_label, 6);

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
    GtkWidget *session_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    gtk_box_append(GTK_BOX(session_row), create_session_card("SRE #1028", "Captain America: Civil War Edit", "41 minutes ago"));
    gtk_box_append(GTK_BOX(session_row), create_session_card("MSE #1034", "Suits 5x8 Ending Music", "2 hours ago"));
    gtk_box_append(GTK_BOX(session_row), create_session_card("SRE #1039", "James Bond: Skyfall Edit", "Yesterday"));
    gtk_box_append(GTK_BOX(session_row), create_session_card("SRE #1032", "The Rookie: Dim and Juicy", "3 days ago"));
    gtk_box_append(GTK_BOX(session_row), create_session_card("SRE #1001", "The Day of the Jackal Edit", "Last week"));
    gtk_box_append(GTK_BOX(session_row), create_session_card("MSE #1001", "House of Cards Inauguration Theme", "Last month"));

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), session_row);

    gtk_box_append(GTK_BOX(content), recent_label);
    gtk_box_append(GTK_BOX(content), scroll);

    AdwClamp *clamp = ADW_CLAMP(adw_clamp_new());
    adw_clamp_set_maximum_size(clamp, 700);
    adw_clamp_set_child(clamp, content);

    gtk_box_append(GTK_BOX(landing), GTK_WIDGET(clamp));
    return landing;
}