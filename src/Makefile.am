AM_CFLAGS = -Wall -Wfatal-errors -Wwrite-strings

INCLUDES = $(MSSH_CFLAGS)

bin_PROGRAMS = mssh

mssh_SOURCES = mssh.c mssh-terminal.c mssh-window.c mssh-pref.c \
	mssh-gconf.c
mssh_LDADD = $(MSSH_LIBS)

EXTRA_DIST = mssh-window.h mssh-terminal.h mssh-pref.h mssh-gconf.h
