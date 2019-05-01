// gcc systray.c -o systray `pkg-config --cflags --libs gtk+-2.0`
#include <gtk/gtk.h>
#include <stdlib.h>

#define ICON_SIZE 32
#define BGCOLOR "#555"

GtkStatusIcon *MyIcon;

static GtkWidget *draw_window(GtkWidget *label){
    GtkWidget *layout = gtk_layout_new(NULL, NULL);
    if(label != NULL){
	gtk_layout_put(GTK_LAYOUT(layout), GTK_WIDGET(label), 0, ICON_SIZE / 4);
	gtk_widget_show(GTK_WIDGET(label));
    }
    GtkWidget *window = gtk_offscreen_window_new();
    gtk_widget_set_size_request(window, ICON_SIZE, ICON_SIZE);
    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show_all(window);
    if(label == NULL){
	gtk_window_present(GTK_WINDOW(window));
    }
    GdkColor color;
    gdk_color_parse (BGCOLOR, &color);
    gtk_widget_modify_bg(layout, GTK_STATE_NORMAL, &color);
    return window;
}

static void draw_complete_event(GtkWidget *window, GdkEvent *event, gpointer user_data){
    GdkPixbuf *pix;
    GdkColormap *cmap = gdk_colormap_get_system ();
    pix = gdk_pixbuf_get_from_drawable (NULL, window->window, cmap, 0, 0, 0, 0, ICON_SIZE, ICON_SIZE);
    gtk_status_icon_set_from_pixbuf(MyIcon, pix);
}

int main(int argc, char *argv[])
{
    // gtk_init(&argc, &argv);
    gtk_init(0, NULL);
    
    //GtkStatusIcon *MyIcon;
    MyIcon = gtk_status_icon_new();
    GtkWidget *label = gtk_label_new("teste");
    GtkWidget *window = draw_window(label);

    //GdkPixbuf *pix;
    //GdkColormap *cmap = gdk_colormap_get_system ();
    //pix = gdk_pixbuf_get_from_drawable (NULL, window->window, cmap, 0, 0, 0, 0, ICON_SIZE, ICON_SIZE);
    //MyIcon = gtk_status_icon_new_from_pixbuf(pix);
    
    g_signal_connect(GTK_OBJECT(window), "damage-event", G_CALLBACK(draw_complete_event), NULL);
        
    // MyIcon = gtk_status_icon_new_from_file("launchapps.svg");
    gtk_status_icon_set_visible(MyIcon, TRUE);

    gtk_main();
}
