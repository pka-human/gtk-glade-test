#include <gtk/gtk.h>

GtkBuilder *builder;

void on_button1_clicked(GtkButton *b) {
    GtkWidget *label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));
    GtkAdjustment *adjustment1 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));
    GtkAdjustment *adjustment2 = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment2"));

    gdouble value1 = gtk_adjustment_get_value(adjustment1);
    gdouble value2 = gtk_adjustment_get_value(adjustment2);

    gchar *result_text = g_strdup_printf("%d + %d = %d", (int)value1, (int)value2, (int)(value1 + value2));
    gtk_label_set_text(GTK_LABEL(label1), result_text);
    g_free(result_text);
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
