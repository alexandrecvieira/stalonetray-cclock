// gcc systray.c -o systray `pkg-config --cflags --libs gtk+-2.0`

#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <sys/time.h>

#define ICON_SIZE 32
#define BGCOLOR "#555"
#define FONTCOLOR "white"
#define FONTSIZE "9.5"
#define FONT "Ubuntu Regular"
#define FONTWEIGHT "bold"

GtkStatusIcon *MyIcon;
GtkWidget *label;
GtkWidget *window;

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

static void cb_allocate(GtkWidget *label, GtkAllocation *allocation){
    gtk_widget_set_size_request(label, ICON_SIZE, -1);
}

static gboolean update_clock(){
    struct timeval now;
    struct tm * current_time;
    gettimeofday(&now, NULL);
    current_time = localtime(&now.tv_sec);

    char clock_value[64];
    clock_value[0] = '\0';
    strftime(clock_value, sizeof(clock_value), "%H:%M", current_time);

    const char *format = "<span color='%s' font_weight='%s' font='%s %s'>\%s</span>"; //"<span foreground='white' size='%s'><b>\%s</b></span>";
    char *markup;
    markup = g_markup_printf_escaped(format, FONTCOLOR, FONTWEIGHT, FONT, FONTSIZE, clock_value);
    gtk_label_set_markup(GTK_LABEL(label), markup);
    g_free(markup);

    g_signal_connect(GTK_OBJECT(label), "size-allocate", G_CALLBACK(cb_allocate), NULL);
    
    gtk_window_present(GTK_WINDOW(window));

    return TRUE;
}

int main(int argc, char *argv[])
{
    // gtk_init(&argc, &argv);
    gtk_init(0, NULL);
    
    //GtkStatusIcon *MyIcon;
    MyIcon = gtk_status_icon_new();
    label = gtk_label_new(NULL);
    window = draw_window(label);

    update_clock();
    
    //GdkPixbuf *pix;
    //GdkColormap *cmap = gdk_colormap_get_system ();
    //pix = gdk_pixbuf_get_from_drawable (NULL, window->window, cmap, 0, 0, 0, 0, ICON_SIZE, ICON_SIZE);
    //MyIcon = gtk_status_icon_new_from_pixbuf(pix);
    
    g_signal_connect(GTK_OBJECT(window), "damage-event", G_CALLBACK(draw_complete_event), NULL);

    g_timeout_add_seconds(1, (GSourceFunc) update_clock, NULL);
        
    // MyIcon = gtk_status_icon_new_from_file("launchapps.svg");
    gtk_status_icon_set_visible(MyIcon, TRUE);

    gtk_main();
}
