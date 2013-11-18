#ifndef __MSSH_TERMINAL_H__
#define __MSSH_TERMINAL_H__

#include <gtk/gtk.h>
#include <vte/vte.h>

G_BEGIN_DECLS

#define MSSH_TYPE_TERMINAL              mssh_terminal_get_type()
#define MSSH_TERMINAL(obj)              G_TYPE_CHECK_INSTANCE_CAST(obj, \
    MSSH_TYPE_TERMINAL, MSSHTerminal)
#define MSSH_TERMINAL_CLASS(klass)      G_TYPE_CHECK_CLASS_CAST(klass, \
    MSSH_TERMINAL_TYPE, MSSHTerminalClass)
#define IS_MSSH_TERMINAL(obj)           G_TYPE_CHECK_INSTANCE_TYPE(obj, \
    MSSH_TYPE_TERMINAL)
#define IS_MSSH_TERMINAL_CLASS(klass)   G_TYPE_CHECK_CLASS_TYPE(klass, \
    MSSH_TYPE_TERMINAL)

typedef struct
{
    VteTerminal vte;
    GtkWidget *menu_item;
    char *hostname;
    int started;
    int ended;
} MSSHTerminal;

typedef struct
{
    VteTerminalClass parent_class;

    guint session_closed_signal;
    guint session_focused_signal;

    void (*session_closed)(MSSHTerminal *terminal);
    void (*session_focused)(MSSHTerminal *terminal);
} MSSHTerminalClass;

GType mssh_terminal_get_type(void) G_GNUC_CONST;

GtkWidget* mssh_terminal_new(void);
void mssh_terminal_destroy(MSSHTerminal *terminal);
gboolean mssh_terminal_isactive(MSSHTerminal *terminal);
void mssh_terminal_init_session(MSSHTerminal *terminal, char *hostname);
void mssh_terminal_start_session(MSSHTerminal *terminal, char **env);
void mssh_terminal_send_host(MSSHTerminal *terminal);
void mssh_terminal_send_string(MSSHTerminal *terminal, gchar *string);
void mssh_terminal_send_data(MSSHTerminal *terminal, GdkEventKey *event);

G_END_DECLS

#endif /* __MSSH_TERMINAL_H__ */
