AC_INIT([MultiSSH], [1.2], [brad@brad-smith.co.uk], [mssh])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

AC_PROG_CC

PKG_CHECK_MODULES(MSSH, [gtk+-2.0 vte gconf-2.0])
AC_SUBST(MSSH_CFLAGS)
AC_SUBST(MSSH_LIBS)

AC_PATH_PROG(GCONFTOOL, gconftool-2, no)

if test x"$GCONFTOOL" = xno; then
  AC_MSG_ERROR([gconftool-2 executable not found in your path - should be installed with GConf])
fi

AM_GCONF_SOURCE_2

AC_OUTPUT(
    Makefile
    src/Makefile
)
