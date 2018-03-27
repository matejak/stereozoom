#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([STEREOZOOM], 0.91, [matej.tyc@google.com]) 
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_CONFIG_SRCDIR([src/stereozoom.h])
AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Because of the gstz icon
AC_SUBST([DATA_SUBDIR], 
	 [stereozoom])

# Checks for programs
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG

AM_PROG_AR
AM_PROG_VALAC([0.34], [have_valac='yes'])
AM_PATH_WXCONFIG(2.8.0, [have_wx='yes'])
PKG_CHECK_MODULES([GTK], [gtk+-3.0], [have_gtk='yes'], [true])
AC_PROG_LIBTOOL
LT_PROG_RC

AC_CANONICAL_HOST
AS_IF([test "${host_os:0:5}" = "mingw"],
      [AC_DEFINE([WINDOWS_VERSION],
		 [1],
		 [Define when building for Windows])
       windows_version="yes"])

AC_ARG_ENABLE([gui], AS_HELP_STRING([--enable-gui], [Whether to attempt to build GUI frontends]), [], [enable_gui=auto])

m4_define([WANT_PACKAGE], [m4_do(
	[AC_ARG_WITH([$1], AS_HELP_STRING([--with-$1], [$2]), [], [enable_$1='auto'])
],
	[AS_IF([test "x$with_$1" != "xno"], [PKG_CHECK_MODULES([$1], [$4], [], [true])])
],
	[AS_IF([test "x$with_$1" = "xyes" -a "x$[$1_LIBS]" = "x"], [AC_MSG_ERROR([$3])])
],
	[AM_CONDITIONAL([$1_BACKEND], [test "x$[$1_LIBS]" != "x"])],
)])


m4_define([WANT_FRONTEND], [m4_do(
	[AC_ARG_WITH([$1], AS_HELP_STRING([--with-$1], [$2]), [], [enable_$1='auto'])
],
	[AS_IF([test "x$with_$1" = "xyes" -a "x$have_$1" != "xyes"], [AC_MSG_ERROR([$3])])
],
)])


WANT_PACKAGE([DevIL], [Build with the DevIL image loader], ["Usage of the DevIL loader has been specifically requested, but it seems that we can't find it"], [IL], [DEVIL_LOADER])

WANT_PACKAGE([allegro5], [Build with the Allegro backend], ["Usage of the Allegro5 backend has been specifically requested, but it seems that we can't find it"], [allegro-5 allegro_primitives-5 allegro_font-5], [ALLEGRO5_BACKEND])

WANT_PACKAGE([allegro4], [Build with the legacy Allegro 4.x backend], ["Usage of the legacy Allegro 4.x backend has been specifically requested, but it seems that we can't find it"], [allegro < 5.0], [ALLEGRO4_BACKEND])

WANT_FRONTEND([wx], [Build with the wxWidgets backend], ["Usage of the wxWidgets GUI has been specifically requested, but it seems that we can't find it"])
AM_CONDITIONAL([wx_BACKEND], [test "x$with_wx" != 'xno' -a "x$have_wx" = "xyes"])

WANT_FRONTEND([gtk], [Build with the GTK backend], ["Usage of the GTK GUI has been specifically requested, but it seems that we can't find it"])
AM_CONDITIONAL([gtk_BACKEND], [test "x$with_gtk" != 'xno' -a "x$have_gtk" = "xyes" -a "x$have_valac" = 'xyes'])

AM_CONDITIONAL([WINDOWS_VERSION],
	       [test "x$windows_version" = "xyes" ])

AM_CONDITIONAL([WANT_GUI], 
	       [test "x$enable_gui" != "xno"]) 

AC_LANG([C++])

# Checks for header files.
AC_CHECK_HEADERS([windows.h unistd.h])

m4_ignore([
# Checks for libraries.
AC_CHECK_HEADER([allegro.h],
		[],
		[have_allegro="no"])

# first check for 'official allegro'
AS_IF([test "x$have_allegro" != "xno" -a "x$have_allegro" != "xyes"],
      [AM_PATH_ALLEGRO([4.2.0], 
		       [have_allegro="yes"
			LIBS="$allegro_LIBS $LIBS"]) ]) 

# then check for autotools-supported Allegro
AS_IF([test "x$have_allegro" != "xno" -a "x$have_allegro" != "xyes"],
      [AC_CHECK_LIB([alleg],
                    [main],
                    [have_allegro="yes"
		     AC_MSG_NOTICE([You can safely ignore the Allegro warning you got earlier...])
                     LIBS="-lalleg $LIBS"]) 
       AC_CHECK_LIB([allegro_unsharable],
                    [main],
                    [LIBS="-lalleg_unsharable $LIBS"]) ])

# and then for Windows allegro...
AS_IF([test "x$have_allegro" != "xno" -a "x$have_allegro" != "xyes"],
      [AC_CHECK_LIB([alleg42],
                    [main],
                    [have_allegro="yes"
                     LIBS="-lalleg42 $LIBS"]) ])
 
AS_IF([test "x$have_allegro" != "xyes"],
      [AC_MSG_ERROR([Unable to find Allegro game programming library 4.x, check out www.allegro.cc (or your distro repositories if you use a unix-like system)])])

# ... subsequently declare we want to build the gui frontend
AC_MSG_CHECKING([whether to build GUI frontend])
AC_MSG_RESULT([$enable_gui])

AC_CHECK_HEADER([IL/il.h],
		[AC_CHECK_LIB([IL], 
			      [ilInit],
			      [have_IL="yes"
			       LIBS="-lIL $LIBS"])
		dnl Some problems ocuured when searching for ilInit with mingw32
                 AC_CHECK_LIB([DevIL],
		              [main],
			      [have_IL="yes"
			       LIBS="-lDevIL $LIBS"]) ])
AS_IF([test "x$have_IL" != "xyes"],
      [AC_MSG_ERROR([You don't have the IL/il.h header file (and/or the IL library). Make sure you have DevIL image loading library properly installed (see http://openil.sourceforge.net)]) ])

])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_C_VOLATILE

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 bin/Makefile
		 docs/Makefile
		 tests/Makefile
		 rsrc/Makefile])

AC_OUTPUT
