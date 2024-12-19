#include <gtk/gtk.h>

GtkBuilder *builder;

void on_button1_clicked (GtkButton *b) {
    GtkWidget *label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    gtk_label_set_text(GTK_LABEL(label1), (const gchar*) "Hello, World!");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_resource("/gui/main.glade");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //gtk_builder_connect_signals(builder, NULL);

    g_signal_connect(gtk_builder_get_object(builder, "button1"), "clicked", G_CALLBACK(on_button1_clicked), NULL);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}
