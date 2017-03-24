#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([STEREOZOOM2], 0.9, [matej.tyc@google.com]) 
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_CONFIG_SRCDIR([src/stereozoom2.h])
AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

# Because of the gstz icon
AC_SUBST([DATA_SUBDIR], 
	 [stereozoom2])

# Checks for programs
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG

AM_PROG_AR
AC_PROG_LIBTOOL
LT_PROG_RC

AC_CANONICAL_HOST
AS_IF([test "${host_os:0:5}" = "mingw"],
      [AC_DEFINE([WINDOWS_VERSION],
		 [1],
		 [Define when building for Windows])
       windows_version="yes"])


AC_ARG_WITH(
	[allegro4], 
	AS_HELP_STRING([--with-allegro4], [Build with the legacy Allegro 4.x backend]), 
	[], [enable_allegro4='auto'])

AC_ARG_WITH(
	[allegro], 
	AS_HELP_STRING([--with-allegro], [Build with the Allegro 5.x backend]),
	[], [enable_allegro='auto'])

AC_ARG_WITH(
	[sdl], 
	AS_HELP_STRING([--with-sdl], [Build with the SDL backend])
	[], [enable_sdl='auto'])

AC_ARG_WITH(
	[wx], 
	AS_HELP_STRING([--with-wx], [Build with the wxWidgets GUI]),
	[], [enable_wx='auto'])

AS_IF([test "x$with_allegro4" != "xno"],
      [PKG_CHECK_MODULES([ALLEGRO4], [allegro<5])])

AM_CONDITIONAL([ALLEGRO4_BACKEND],
	       [test "x$ALLEGRO4_LIBS" != "x" ])

m4_define([WANT_PACKAGE], [m4_do(
	[m4_pushdef([_UPPERCASE], m4_translit([[$1]], a-z, A-Z))],
	[AC_ARG_WITH([$1], AS_HELP_STRING([--with-$1], [$2]), [], [enable_$1='auto'])],
	[AS_IF([test "x$with_$1" != "xno"], [PKG_CHECK_MODULES(_UPPERCASE, [$4])])],
	[AS_IF([test "x$with_$1" = "xyes" -a "x$_UPPERCASE[]_LIBS" = "x"], [AC_MSG_ERROR([$3])])],
	[AM_CONDITIONAL(_UPPERCASE[_BACKEND], [test "x$_UPPERCASE[]_LIBS" = "x"])],
	[m4_popdef([_UPPERCASE])],
)])

WANT_PACKAGE([devil], [Build with the DevIL image loader], ["Usage of the DevIL loader has been specifically requested, but it seems that we can't find it"], [IL], [DEVIL_LOADER])
WANT_PACKAGE([allegro], [Build with the Allegro backend], ["Usage of the Allegro5 backend has been specifically requested, but it seems that we can't find it"], [IL], [DEVIL_LOADER])

AM_CONDITIONAL([WINDOWS_VERSION],
	       [test "x$windows_version" = "xyes" ])

# Checks for user preferences
AC_ARG_ENABLE([gui], 
	      [AS_HELP_STRING([--disable-gui], 
			      [Disables build of wxwidgets GUI (default = yes)] )], 
	      [], 
	      [enable_gui="auto"])

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
      [AC_MSG_ERROR([Unable to find Allegro game programming library 4.x, check out www.allegro.cc (or your distro repositories if you use a unix-like system)]) ])

# If the user isn't against it, try to use wxwidgets, and ...
AS_IF([test "x$enable_gui" != "xno"], 
      [AM_PATH_WXCONFIG(2.8.0, [HAVE_WXWIDGETS=1]) ])

# Define the symbol so that Allegro drops the "magic main" stuff and the user is reported on how the attempt to build GUI has ended
AS_IF([test "x$enable_gui" != "xno" -a "x$HAVE_WXWIDGETS" != "x1"], 
      [enable_gui="no (suitable wxWidgets installation not found)"], 
      [test "x$enable_gui" != "xno" -a "x$HAVE_WXWIDGETS" = "x1"], 
      [AC_DEFINE([WANT_GUI], 
		 1, 
		 [Define if you want to build a wxWidgets GUI]) ])

# ... subsequently declare we want to build the gui frontend
AM_CONDITIONAL([WANT_GUI], 
	       [test "x$HAVE_WXWIDGETS" = "x1"]) 

AC_MSG_CHECKING([whether to build GUI frontend])
AC_MSG_RESULT([$enable_gui])

AC_LANG([C++])

# Checks for header files.
AC_CHECK_HEADERS([windows.h unistd.h])

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


# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_C_VOLATILE

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 bin/Makefile
		 docs/Makefile
		 rsrc/Makefile])

AC_OUTPUT
