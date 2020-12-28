dnl# glade-1.1.3/examples/editor/configure.ac                  -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.70])
AC_INIT([glade-editor],[0.1],[damon@gnome.org])
AC_CONFIG_SRCDIR([acconfig.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AM_SANITY_CHECK

AC_USE_SYSTEM_EXTENSIONS
AC_SEARCH_LIBS([strerror],[cposix])
if test "x${CC}" = "x"; then
  test -z "${CC}"
  AC_PROG_CC
else
  test -n "${CC}" && export CC
fi
AC_PROG_CPP
AC_PROG_CXX
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
IT_PROG_INTLTOOL

# Checks for header files.
AC_CHECK_HEADERS([libintl.h])dnl

if test "x${ac_cv_header_string_h}" = "x"; then
  test -z "${ac_cv_header_string_h}"
  AC_CHECK_HEADERS([string.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}"
  AC_CHECK_HEADERS([unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

if test -d /Users/Shared/Previously\ Relocated\ Items/Security/sw/bin; then
  echo "adding /Users/Shared/Previously\ Relocated\ Items/Security/sw/bin to PATH"
  export PATH="${PATH}:/Users/Shared/Previously\ Relocated\ Items/Security/sw/bin"
fi
AM_PATH_GTK([1.2.0],[ ],
            [AC_MSG_WARN([Cannot find GTK: Is gtk-config in path?])
             AC_MSG_NOTICE([PATH is ${PATH}])
             AC_MSG_NOTICE([(this ought to be an error, but we are turning it into a warning to facilitate work on this configure script)])
             AC_PATH_PROGS([LOCATE_BIN],[locate glocate],[locate])
             AC_CHECK_PROGS([HEAD_BIN],[head ghead],[head],[head])
             POSSIBLE_GTK_CONFIG="`${LOCATE_BIN} bin/gtk-config | ${HEAD_BIN} -n 1`"
             if test "x${POSSIBLE_GTK_CONFIG}" != "x"; then
               test -n "${POSSIBLE_GTK_CONFIG}" && test -e "${POSSIBLE_GTK_CONFIG}" && stat "${POSSIBLE_GTK_CONFIG}"
               if test -x "${POSSIBLE_GTK_CONFIG}"; then
                 AC_MSG_NOTICE([found possible gtk-config at ${POSSIBLE_GTK_CONFIG}; consider adding its parent directory to your PATH])
               else
                 AC_MSG_NOTICE([found possible gtk-config at ${POSSIBLE_GTK_CONFIG}, but is probably invalid; ignoring])
               fi
             fi
])dnl

dnl# Add the languages which your application supports here.
ALL_LINGUAS=""
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])dnl

dnl# Set PACKAGE_LOCALE_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED([PACKAGE_LOCALE_DIR],["${ac_default_prefix}/${DATADIRNAME}/locale"])
else
  AC_DEFINE_UNQUOTED([PACKAGE_LOCALE_DIR],["${prefix}/${DATADIRNAME}/locale"])
fi

dnl# Set PACKAGE_DATA_DIR in config.h.
if test "x${datadir}" = 'x${prefix}/share'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED([PACKAGE_DATA_DIR],["${ac_default_prefix}/share/${PACKAGE}"])
  else
    AC_DEFINE_UNQUOTED([PACKAGE_DATA_DIR],["${prefix}/share/${PACKAGE}"])
  fi
else
  AC_DEFINE_UNQUOTED([PACKAGE_DATA_DIR],["${datadir}/${PACKAGE}"])
fi

dnl# Set PACKAGE_SOURCE_DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`
AC_DEFINE_UNQUOTED([PACKAGE_SOURCE_DIR],["${packagesrcdir}"])dnl

dnl# Use -Wall if we have gcc.
changequote(,)dnl
if test "x${GCC}" = "xyes"; then
  case " ${CFLAGS} " in
  *[\ \	]-Wall[\ \	]*) ;;
  *) CFLAGS="${CFLAGS} -Wall" ;;
  esac
fi
changequote([,])dnl

# Output.
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 po/Makefile.in])
AC_OUTPUT
