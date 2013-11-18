#include <vte/vte.h>
#include <gdk/gdkkeysyms.h>

#include "mssh-gconf.h"
#include "mssh-window.h"
#include "mssh-terminal.h"

void mssh_gconf_notify_font(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;
    const gchar *font;
    int i;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);
    font = gconf_value_get_string(value);

    for(i = 0; i < window->terminals->len; i++)
    {
        vte_terminal_set_font_from_string(VTE_TERMINAL(g_array_index(
            window->terminals, MSSHTerminal*, i)), font);
    }
}

void mssh_gconf_notify_fg_colour(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;
    const gchar *colour_s;
    GdkVisual *visual = gdk_visual_get_system();
    GdkColormap *colour_map;
    GdkColor colour;
    int i;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);
    colour_s = gconf_value_get_string(value);
    colour_map = gdk_colormap_new(visual, TRUE);
    gdk_colormap_alloc_color(colour_map, &colour, TRUE, TRUE);

    gdk_color_parse(colour_s, &colour);

    for(i = 0; i < window->terminals->len; i++)
    {
        vte_terminal_set_color_foreground(VTE_TERMINAL(g_array_index(
            window->terminals, MSSHTerminal*, i)), &colour);
    }
}

void mssh_gconf_notify_bg_colour(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;
    const gchar *colour_s;
    GdkVisual *visual = gdk_visual_get_system();
    GdkColormap *colour_map;
    GdkColor colour;
    int i;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);
    colour_s = gconf_value_get_string(value);
    colour_map = gdk_colormap_new(visual, TRUE);
    gdk_colormap_alloc_color(colour_map, &colour, TRUE, TRUE);

    gdk_color_parse(colour_s, &colour);

    for(i = 0; i < window->terminals->len; i++)
    {
        vte_terminal_set_color_background(VTE_TERMINAL(g_array_index(
            window->terminals, MSSHTerminal*, i)), &colour);
    }
}

void mssh_gconf_notify_columns(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;
    int columns;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);
    columns = gconf_value_get_int(value);

    if(columns <= 0)
    {
        columns = 1;
        gconf_client_set_int(client, MSSH_GCONF_KEY_COLUMNS, columns,
            NULL);
    }

    window->columns = columns;
    mssh_window_relayout(window);
}

void mssh_gconf_notify_timeout(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;
    int timeout;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);
    timeout = gconf_value_get_int(value);

    if(timeout < 0)
    {
        timeout = 0;
        gconf_client_set_int(client, MSSH_GCONF_KEY_TIMEOUT, timeout,
            NULL);
    }

    window->timeout = timeout;
    mssh_window_relayout(window);
}

void mssh_gconf_notify_close_ended(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;
    gboolean close_ended;
    int i;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);
    close_ended = gconf_value_get_bool(value);

    window->close_ended_sessions = close_ended;

    if(close_ended)
    {
        for(i = 0; i < window->terminals->len; i++)
        {
            MSSHTerminal *terminal = g_array_index(window->terminals,
                MSSHTerminal*, i);

            if(terminal->ended)
            {
                mssh_window_session_closed(terminal, window);
            }
        }
    }
}

void mssh_gconf_notify_quit_all_ended(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);

    window->exit_on_all_closed = gconf_value_get_bool(value);
}

void mssh_gconf_notify_dir_focus(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);

    window->dir_focus = gconf_value_get_bool(value);
}

void mssh_gconf_notify_modifier(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data)
{
    GConfValue *value;

    MSSHWindow *window = MSSH_WINDOW(data);

    value = gconf_entry_get_value(entry);

    if(window->accel)
    {
        gtk_accel_group_disconnect_key(window->accel, GDK_Up,
            window->modifier);
        gtk_accel_group_disconnect_key(window->accel, GDK_Down,
            window->modifier);
        gtk_accel_group_disconnect_key(window->accel, GDK_Left,
            window->modifier);
        gtk_accel_group_disconnect_key(window->accel, GDK_Right,
            window->modifier);
    }

    window->modifier = gconf_value_get_int(value);

    if(window->accel)
    {
        gtk_accel_group_connect(window->accel, GDK_Up, window->modifier,
            GTK_ACCEL_VISIBLE, g_cclosure_new(
            G_CALLBACK(mssh_window_focus), window, NULL));
        gtk_accel_group_connect(window->accel, GDK_Down, window->modifier,
            GTK_ACCEL_VISIBLE, g_cclosure_new(
            G_CALLBACK(mssh_window_focus), window, NULL));
        gtk_accel_group_connect(window->accel, GDK_Left, window->modifier,
            GTK_ACCEL_VISIBLE, g_cclosure_new(
            G_CALLBACK(mssh_window_focus), window, NULL));
        gtk_accel_group_connect(window->accel, GDK_Right, window->modifier,
            GTK_ACCEL_VISIBLE, g_cclosure_new(
            G_CALLBACK(mssh_window_focus), window, NULL));
    }
}
