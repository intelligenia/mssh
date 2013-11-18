#ifndef __MSSH_PREF_H__
#define __MSSH_PREF_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define MSSH_TYPE_PREF              mssh_pref_get_type()
#define MSSH_PREF(obj)              G_TYPE_CHECK_INSTANCE_CAST(obj,\
    MSSH_TYPE_PREF, MSSHPref)
#define MSSH_PREF_CLASS(klass)      G_TYPE_CHECK_CLASS_CAST(klass,\
    MSSH_PREF_TYPE, MSSHPrefClass)
#define IS_MSSH_PREF(obj)           G_TYPE_CHECK_INSTANCE_TYPE(obj,\
    MSSH_TYPE_PREF)
#define IS_MSSH_PREF_CLASS(klass) G_TYPE_CHECK_CLASS_TYPE(klass,\
    MSSH_TYPE_PREF)

typedef struct
{
    GtkWindow widget;
    GtkWidget *ctrl;
    GtkWidget *alt;
    GtkWidget *shift;
    GtkWidget *super;
} MSSHPref;

typedef struct
{
    GtkWindowClass parent_class;
} MSSHPrefClass;

GType mssh_pref_get_type(void) G_GNUC_CONST;

GtkWidget* mssh_pref_new(void);

G_END_DECLS

#endif /* __MSSH_PREF_H__ */
