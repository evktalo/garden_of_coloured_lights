#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.62)
AC_INIT([GARDEN_OF_COLOURED_LIGHTS], [1.0.2], [BUG-REPORT-ADDRESS])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/input.h])
AC_CONFIG_HEADERS([src/configure.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AM_PATH_ALLEGRO([4.2.2])

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_HEADER_STDBOOL

# Checks for library functions.

# Sets the data subdirectory
AC_SUBST([DATADIR_NAME],
	 [garden])

AC_CONFIG_FILES([Makefile
                 bin/Makefile
		 data/Makefile])

AC_OUTPUT
