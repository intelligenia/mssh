#ifndef __MSSH_GCONF__
#define __MSSH_GCONF__

#include <gconf/gconf-client.h>

#define MSSH_GCONF_PATH                 "/apps/mssh"
#define MSSH_GCONF_KEY_FONT             MSSH_GCONF_PATH"/font"
#define MSSH_GCONF_KEY_FG_COLOUR        MSSH_GCONF_PATH"/fg_colour"
#define MSSH_GCONF_KEY_BG_COLOUR        MSSH_GCONF_PATH"/bg_colour"
#define MSSH_GCONF_KEY_COLUMNS          MSSH_GCONF_PATH"/columns"
#define MSSH_GCONF_KEY_TIMEOUT          MSSH_GCONF_PATH"/timeout"
#define MSSH_GCONF_KEY_CLOSE_ENDED      MSSH_GCONF_PATH"/close_ended"
#define MSSH_GCONF_KEY_QUIT_ALL_ENDED   MSSH_GCONF_PATH"/quit_all_ended"
#define MSSH_GCONF_KEY_MODIFIER         MSSH_GCONF_PATH"/modifier"
#define MSSH_GCONF_KEY_DIR_FOCUS        MSSH_GCONF_PATH"/dir_focus"

void mssh_gconf_notify_font(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_fg_colour(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_bg_colour(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_columns(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_timeout(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_close_ended(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_quit_all_ended(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_modifier(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);
void mssh_gconf_notify_dir_focus(GConfClient *client, guint cnxn_id,
    GConfEntry *entry, gpointer data);

#endif
