#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([gnome-radio],[50.1])
AM_INIT_AUTOMAKE([-Wno-portability 1.11 no-dist-gzip dist-xz tar-ustar subdir-objects])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/gnome-radio.c])
AC_CONFIG_HEADERS([config.h])

LT_PREREQ([2.2.6])
LT_INIT([dlopen win32-dll pic-only])

m4_ifdef([AM_SILENT_RULES],
  [AM_SILENT_RULES([yes])],
  [
    AM_DEFAULT_VERBOSITY=1
    AC_SUBST(AM_DEFAULT_VERBOSITY)
  ]
)

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_CC_C_O

AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_C_CONST
AC_C_INLINE
AC_SEARCH_LIBS([strerror],[cposix])
AC_SYS_LARGEFILE

AC_C_BIGENDIAN

LT_LIB_M
AC_SUBST(LIBM)

PKG_PROG_PKG_CONFIG

# Checks for libraries.

help_msg="no (not requested)"
try_help=:
gnome_internet_radio_locator_help=false

AC_ARG_WITH([help],AS_HELP_STRING([--with-help],[Support GNOME Radio Help]), [
     AC_MSG_CHECKING([for help])
     if test x$withval = xno; then
           try_help=false
	   help_msg = "no (not requested)"
	   AC_DEFINE([ENABLE_USER_HELP],0,[help missing])
	   gnome_internet_radio_locator_help=false
     else
	   AC_DEFINE([ENABLE_USER_HELP],1,[support help])
	   help_msg="yes (with help support)"
	   gnome_internet_radio_locator_help=true
     fi
     AC_MSG_RESULT([$help_msg])
])

AM_CONDITIONAL([ENABLE_USER_HELP], [$gnome_internet_radio_locator_help])
AC_SUBST([ENABLE_USER_HELP])

record_msg="no (not requested)"
try_record=:
gnome_internet_radio_locator_record=false

AC_ARG_WITH([recording],AS_HELP_STRING([--with-recording],[Support recording with gstreamer]), [
     AC_MSG_CHECKING([for recording])
     if test x$withval = xno; then
           try_record=false
	   record_msg = "no (not requested)"
	   AC_DEFINE([HAVE_GNOME_RADIO_RECORD],0,[record missing])
	   gnome_internet_radio_locator_record=false
     else
	   AC_DEFINE([HAVE_GNOME_RADIO_RECORD],1,[have record])
	   record_msg="yes (with recording support)"
	   gnome_internet_radio_locator_record=true
     fi
     AC_MSG_RESULT([$record_msg])
])
AM_CONDITIONAL([HAVE_GNOME_RADIO_RECORD], [$gnome_internet_radio_locator_record])
AC_SUBST([HAVE_GNOME_RADIO_RECORD])

AC_CHECK_LIB(m, pow, [], [], [])

PKG_CHECK_MODULES(GNOME_RADIO,
		  champlain-gtk-0.12 >= 0.12.10 \
		  geocode-glib-2.0 >= 3.26 \
		  glib-2.0 >= 2.38.0 \
		  gobject-2.0 >= 2.38.0 \
		  gstreamer-1.0 >= 1.0 \
		  gstreamer-player-1.0 >= 1.0 \
		  gstreamer-tag-1.0 >= 1.0 \
		  gstreamer-video-1.0 \
		  gstreamer-plugins-bad-1.0 \
		  gstreamer-plugins-base-1.0 \
		  gtk+-3.0 >= 3.24.34 \
		  geoclue-2.0 >= 2.4.12 \
		  libgeoclue-2.0 >= 2.4.12 \
		  libxml-2.0 >= 2.0 \
		  pangoft2 >= 0.28)

GNOME_RADIO_PKG_DEPS="champlain-gtk-0.12 geocode-glib-2.0 glib-2.0 gobject-2.0 gstreamer-1.0 gstreamer-player-1.0 gstreamer-plugins-bad-1.0 gstreamer-plugins-base-1.0 gstreamer-plugins-good-1.0 gstreamer-video-1.0 gtk+-3.0 geoclue-2.0 libgeoclue-2.0 libxml-2.0 pangoft2"

GTK_DOC_CHECK([1.16], [--flavour no-tmpl])

YELP_HELP_INIT

# Checks for header files.
m4_warn([obsolete],
[The preprocessor macro 'STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  C89 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

# ***********                                                                   
# Translation                                                                   
# ***********                                                                   

AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.50.1])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext package.])

AC_SUBST(GNOME_RADIO_LIBS)
AC_SUBST(GNOME_RADIO_CFLAGS)
AC_SUBST(GNOME_RADIO_PKG_DEPS)

dnl Enable yelp-tools
# YELP_HELP_INIT

AC_CONFIG_FILES([
Makefile
data/Makefile
data/icons/Makefile
data/gnome-radio.desktop
doc/Makefile
gtk/Makefile
help/Makefile
help/C/version.xml
man/Makefile
po/Makefile.in
src/Makefile
gnome-radio.spec
])
AC_OUTPUT
