dnl# glade-1.1.3/configure.ac                                  -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([glade],[1.1.3],[damon@gnome.org])
AC_CONFIG_SRCDIR([glade/gbwidget.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_REQUIRE_AUX_FILE([mkinstalldirs])dnl

AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])dnl

AC_DEFINE([THIS_IS_THE_CONFIG_H_FOR_GLADE],[1],[Define to 1 here])dnl

AC_SEARCH_LIBS([strerror],[cposix])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX

IT_PROG_INTLTOOL

LT_INIT

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

GTK_DOC_CHECK

# Checks for header files.
dnl# autoupdate says:
dnl# The preprocessor macro `STDC_HEADERS' is obsolete.
dnl# Except in unusual embedded environments, you can safely include all
dnl# ISO C90 headers unconditionally.
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
if test "x${EGREP}" = "x"; then
  test -z "${EGREP}"
  AC_PROG_EGREP
fi
AC_CHECK_HEADERS([libintl.h locale.h os2.h stdlib.h string.h unistd.h])dnl

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([mkdir putenv setlocale strchr strrchr strstr strtoul tzset])dnl

AC_PATH_XTRA

PKG_PROG_PKG_CONFIG

dnl#***************************************************************************
dnl# pkg-config - check for gtk
dnl#***************************************************************************

glade_modules=""
gtk_modules="libxml-2.0 >= 2.4.1 gtk+-2.0 >= 2.0.0"

PKG_CHECK_MODULES([GLADE_GTK],[${gtk_modules}])

glade_modules="${glade_modules} ${gtk_modules}"


dnl#***************************************************************************
dnl# Gnome Support (use --disable-gnome to disable).
dnl# (Use "#ifdef USE_GNOME" in Glade for Gnome-specific code.)
dnl#***************************************************************************

gnome_modules="libgnomeui-2.0 >= 2.0.0 libgnomecanvas-2.0 >= 2.0.0 libbonoboui-2.0 >= 2.0.0"

AC_ARG_ENABLE([gnome],
[AS_HELP_STRING([--disable-gnome],[disable use of gnome])],
	[gnome=${enableval}],[gnome=yes])dnl

if test "x${gnome}" = "xyes"; then
  PKG_CHECK_MODULES([GLADE_GNOME],[${gnome_modules}],[
    AC_DEFINE([USE_GNOME])
    GLADE_GNOME_DIR="gnome"
    GLADE_GNOME_LIB="gnome/libgnomewidgets.a"
    AC_SUBST([GLADE_GNOME_DIR])
    AC_SUBST([GLADE_GNOME_LIB])
    glade_modules="${glade_modules} ${gnome_modules}"
  ],[gnome=no])
fi


dnl#***************************************************************************
dnl# Gnome-DB Support (use --disable-gnome-db to disable).
dnl#***************************************************************************

gnomedb_modules="libgda >= 0.8.102 libgnomedb >= 0.8.102"

AC_ARG_ENABLE([gnome-db],
[AS_HELP_STRING([--disable-gnome-db],[disable use of gnome-db])],
	[gnome_db=${enableval}],[gnome_db=yes])

if test "x${gnome}" != "xyes"; then
  AC_MSG_WARN([GNOME support is disabled; GNOME DB support will not be built.])
  gnome_db=no
fi

if test "x${gnome_db}" = "xyes"; then
  PKG_CHECK_MODULES([GLADE_GNOMEDB],[${gnomedb_modules}],[
    AC_DEFINE([USE_GNOME_DB])
    GLADE_GNOME_DB_DIR="gnome-db"
    GLADE_GNOME_DB_LIB="gnome-db/libgnomedbwidgets.a"
    AC_SUBST([GLADE_GNOME_DB_DIR])
    AC_SUBST([GLADE_GNOME_DB_LIB])
    glade_modules="${glade_modules} ${gnomedb_modules}"
  ],[gnome_db=no])
fi


dnl#***************************************************************************
dnl# Final pkgconfig run: this runs with all of the modules we need
dnl#***************************************************************************
PKG_CHECK_MODULES([GLADE],[${glade_modules}])
GLADE_CFLAGS="${GLADE_CFLAGS} ${X_CFLAGS}"
GLADE_LIBS="${GLADE_LIBS} ${X_LIBS} ${X_PRE_LIBS} -lX11 ${X_EXTRA_LIBS}"
AC_SUBST([GLADE_CFLAGS])dnl
AC_SUBST([GLADE_LIBS])dnl

SAVE_CFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} ${GLADE_CFLAGS}"
AC_CHECK_HEADERS([gdk/gdkx.h])
AC_CHECK_FUNCS([bonobo_dock_item_get_floating_position \
                bonobo_dock_item_detach glade_gnome_setup_initial_menus])
CFLAGS="${SAVE_CFLAGS}"


dnl#***************************************************************************
dnl# Deprecation flags.
dnl#***************************************************************************

GLADE_DEPRECATION_CFLAGS=""
if test "x${BE_ANNOYING_AND_DISABLE_DEPRECATED_STUFF}" = "xyes"; then
    GLADE_DEPRECATION_CFLAGS="-DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGNOME_DISABLE_DEPRECATED -Wall -Werror"
fi
AC_SUBST([GLADE_DEPRECATION_CFLAGS])dnl


dnl#***************************************************************************
dnl# Gettext stuff.
dnl#***************************************************************************

GETTEXT_PACKAGE=glade-2.0
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["${GETTEXT_PACKAGE}"])dnl

ALL_LINGUAS="az ca cs da de el en_GB es fr ga gl hu it ja ko lv nn no pl pt pt_BR ro ru sk sv tr uk vi zh_CN"
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])
AM_GLIB_GNU_GETTEXT


dnl#***************************************************************************
dnl# Check for ScrollKeeper
dnl#***************************************************************************

AC_PATH_PROG([SCROLLKEEPER_CONFIG],[scrollkeeper-config],[no])
if test "x${SCROLLKEEPER_CONFIG}" = "xno"; then
  AC_MSG_ERROR([Failed to find scrollkeeper-config, please install the scrollkeeper package])
fi
SCROLLKEEPER_REQUIRED=0.1.4
AC_SUBST([SCROLLKEEPER_REQUIRED])dnl


dnl#***************************************************************************
dnl# Palette type - temporary option to try the different palette layouts.
dnl#***************************************************************************

AC_ARG_WITH([palette],
[AS_HELP_STRING([--with-palette=TYPE],
                [select the palette type: damon or martijn])],
	[if test "x${withval}" = "xmartijn"; then
	   PALETTE_TYPE=2
	   AC_MSG_RESULT([Picking Martijn's palette layout])
	 else
	   PALETTE_TYPE=1
	   AC_MSG_RESULT([Picking Damon's palette layout])
	 fi],
	[PALETTE_TYPE=1])
AC_DEFINE_UNQUOTED([PALETTE_TYPE],[${PALETTE_TYPE}])dnl
	

dnl#***************************************************************************
dnl# Debugging - defines GLADE_DEBUG if --enable-debug option is used.
dnl#***************************************************************************

AC_ARG_ENABLE([debug],
[AS_HELP_STRING([--enable-debug],[turn on debugging [default=no]])],
	[AC_DEFINE([GLADE_DEBUG])],[])dnl


dnl#***************************************************************************
dnl# Output the Makefiles etc.
dnl#***************************************************************************

AC_CONFIG_FILES([glade.spec
                 Makefile
                 glade/Makefile
                 glade/gbwidgets/Makefile
                 glade/gnome/Makefile
                 glade/gnome-db/Makefile
                 glade/data/Makefile
                 glade/data/gtk/Makefile
                 doc/Makefile
                 doc/C/Makefile
                 doc/C/glade-faq/Makefile
                 doc/C/glade-turbo-start/Makefile
                 doc/C/glade-user-guide/Makefile
                 omf-install/Makefile
                 po/Makefile.in])
AC_CONFIG_SUBDIRS([examples/editor])
AC_OUTPUT

dnl# maybe also add doc/it/Makefile

echo "

Configuration:

	Source code location:	${srcdir}
	Compiler:		${CC} 

	Gnome Support:		${gnome}
	Gnome DB Support:	${gnome_db}
"
