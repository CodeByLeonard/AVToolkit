#include "sre_page.h"

#include <adwaita.h>
#include <gtk/gtk.h>
#include <stdio.h>

/* =========================================================
   HELPERS
========================================================= */

static GtkWidget* create_video_placeholder(const char *label, int w, int h) {
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_widget_set_size_request(frame, w, h);
    gtk_widget_add_css_class(frame, "card");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);

    GtkWidget *l = gtk_label_new(label);
    gtk_widget_add_css_class(l, "dim-label");

    gtk_box_append(GTK_BOX(box), l);
    gtk_frame_set_child(GTK_FRAME(frame), box);

    return frame;
}

static GtkWidget* create_divider() {
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_margin_top(sep, 10);
    gtk_widget_set_margin_bottom(sep, 10);
    return sep;
}

static GtkWidget* create_step_label(int step, const char *title) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Step %d · %s", step, title);

    GtkWidget *label = gtk_label_new(buffer);
    gtk_widget_add_css_class(label, "title-4");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}

/* =========================================================
   SEGMENT CARD
========================================================= */

static GtkWidget* create_segment_card(int index) {

    GtkWidget *card = gtk_frame_new(NULL);
    gtk_widget_add_css_class(card, "card");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(box, 10);
    gtk_widget_set_margin_bottom(box, 10);
    gtk_widget_set_margin_start(box, 10);
    gtk_widget_set_margin_end(box, 10);

    char num[32];
    snprintf(num, sizeof(num), "#%d", index + 1);

    GtkWidget *badge = gtk_label_new(num);
    gtk_widget_add_css_class(badge, "title-4");
    gtk_widget_set_halign(badge, GTK_ALIGN_START);

    GtkWidget *time_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

    GtkWidget *start = gtk_entry_new();
    GtkWidget *end   = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(start), "Start");
    gtk_entry_set_placeholder_text(GTK_ENTRY(end), "End");

    gtk_widget_set_hexpand(start, TRUE);
    gtk_widget_set_hexpand(end, TRUE);

    gtk_box_append(GTK_BOX(time_row), start);
    gtk_box_append(GTK_BOX(time_row), end);

    gtk_box_append(GTK_BOX(box), badge);
    gtk_box_append(GTK_BOX(box), time_row);

    gtk_frame_set_child(GTK_FRAME(card), box);

    return card;
}

/* =========================================================
   MAIN PAGE
========================================================= */

GtkWidget* build_sre_page(void) {

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);

    gtk_widget_set_vexpand(root, TRUE);
    gtk_widget_set_hexpand(root, TRUE);

    gtk_widget_set_margin_top(root, 24);
    gtk_widget_set_margin_bottom(root, 24);
    gtk_widget_set_margin_start(root, 24);
    gtk_widget_set_margin_end(root, 24);

    /* ================= HEADER ================= */

    GtkWidget *title = gtk_label_new("SRE · Shorts Reverse Engineering");
    gtk_widget_add_css_class(title, "title-1");
    gtk_widget_set_halign(title, GTK_ALIGN_START);

    GtkWidget *subtitle = gtk_label_new(
        "Steal viral videos from the YouTube Shorts platform"
    );
    gtk_widget_add_css_class(subtitle, "dim-label");
    gtk_widget_set_halign(subtitle, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(root), title);
    gtk_box_append(GTK_BOX(root), subtitle);

    gtk_box_append(GTK_BOX(root), create_divider());
    gtk_box_append(GTK_BOX(root), create_step_label(1, "Inputs"));

    /* ================= INPUT ROW (FIXED LAYOUT) ================= */

    GtkWidget *input_row = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);

    /* LEFT: Shorts input + download */
    GtkWidget *left = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_hexpand(left, TRUE);

    GtkWidget *shorts_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(shorts_entry),
        "YouTube Shorts ID");

    gtk_widget_set_hexpand(shorts_entry, TRUE);

    GtkWidget *download_btn = gtk_button_new_with_label("Download");
    gtk_widget_add_css_class(download_btn, "suggested-action");

    gtk_box_append(GTK_BOX(left), shorts_entry);
    gtk_box_append(GTK_BOX(left), download_btn);

    /* RIGHT: file + search (FIXED ALIGNMENT) */
    GtkWidget *right = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_hexpand(right, TRUE);

    GtkWidget *file_btn = gtk_button_new_with_label("📂 Open File");
    gtk_widget_add_css_class(file_btn, "suggested-action");

    GtkWidget *search = gtk_search_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search), "Search archive...");
    gtk_widget_set_hexpand(search, TRUE);

    gtk_box_append(GTK_BOX(right), file_btn);
    gtk_box_append(GTK_BOX(right), search);

    gtk_box_append(GTK_BOX(input_row), left);
    gtk_box_append(GTK_BOX(input_row), right);

    gtk_box_append(GTK_BOX(root), input_row);

    /* ================= PREVIEWS (FIXED RATIO LOGIC) ================= */

    GtkWidget *preview_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 24);
    gtk_widget_set_halign(preview_row, GTK_ALIGN_FILL);

    /* FIXED BALANCE: medium wider, short slimmer, same height */
    GtkWidget *short_preview =
        create_video_placeholder("Short (9:16)", 160, 285);

    GtkWidget *medium_preview =
        create_video_placeholder("Medium (16:9)", 480, 285);

    gtk_widget_set_hexpand(short_preview, TRUE);
    gtk_widget_set_hexpand(medium_preview, TRUE);

    gtk_box_append(GTK_BOX(preview_row), short_preview);
    gtk_box_append(GTK_BOX(preview_row), medium_preview);

    gtk_box_append(GTK_BOX(root), preview_row);

    /* ================= SCOPING ================= */

    gtk_box_append(GTK_BOX(root), create_divider());
    gtk_box_append(GTK_BOX(root), create_step_label(2, "Scoping"));

    GtkWidget *scope_desc = gtk_label_new(
        "Define segments of interest for matching"
    );
    gtk_widget_add_css_class(scope_desc, "dim-label");
    gtk_widget_set_halign(scope_desc, GTK_ALIGN_START);

    gtk_box_append(GTK_BOX(root), scope_desc);

    /* GRID */
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    int cols = 3;

    for (int i = 0; i < 6; i++) {
        GtkWidget *card = create_segment_card(i);

        int row = i / cols;
        int col = i % cols;

        gtk_grid_attach(GTK_GRID(grid), card, col, row, 1, 1);
    }

    gtk_box_append(GTK_BOX(root), grid);

    /* FOOTER */
    GtkWidget *footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_hexpand(footer, TRUE);

    GtkWidget *add_btn = gtk_button_new_with_label("+ Add Segment");

    GtkWidget *start_btn = gtk_button_new_with_label("Start Matching");
    gtk_widget_add_css_class(start_btn, "suggested-action");
    gtk_widget_set_hexpand(start_btn, TRUE);
    gtk_widget_set_halign(start_btn, GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(footer), add_btn);
    gtk_box_append(GTK_BOX(footer), start_btn);

    gtk_box_append(GTK_BOX(root), footer);

    gtk_box_append(GTK_BOX(root), create_divider());
    gtk_box_append(GTK_BOX(root), create_step_label(3, "Processing"));

    /* ================= SCROLL ================= */

    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_NEVER,
        GTK_POLICY_AUTOMATIC
    );

    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), root);

    return scroll;
}