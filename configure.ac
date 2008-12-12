#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.62)
AC_INIT([GARDEN_OF_COLOURED_LIGHTS], [1.0.5], [bubla@users.sourceforge.net])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/input.h])
AC_CONFIG_HEADERS([src/configure.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
LT_PROG_RC

AC_CANONICAL_HOST

AS_CASE([$host],
        [*mingw* | *cygwin*],
        [AC_DEFINE([WINDOWS_VERSION],
 		   [1],
		   [Define when building for Windows])
         windows_version="yes"])

AM_CONDITIONAL([WINDOWS_VERSION],
	       [test "x$windows_version" = "xyes" ])

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
      [AC_MSG_ERROR([Unable to find Allegro game programming library 4.2, check out www.allegro.cc (or your distro repositories if you use a unix-like system)]) ])

# Checks for header files.
AC_CHECK_HEADERS([string.h sys/stat.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_HEADER_STDBOOL

# Checks for library functions.

# Sets the data subdirectory
AC_SUBST([DATADIR_NAME],
	 [garden])

AC_CONFIG_FILES([Makefile
                 bin/Makefile
		 data/Makefile
		 resources/Makefile
		 pkgs/w32/winstaller.nsi])

AC_OUTPUT
