#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
m4_define([VERSION],
          m4_esyscmd([build-aux/git-version-gen .tarball-version]))
AC_INIT([x264],
        m4_defn([VERSION]),
        [https://github.com/Freed-Wu/x264-dsp/issues],
        [x264-]m4_defn([VERSION]),
        [https://github.com/Freed-Wu/x264-dsp])
AC_CONFIG_SRCDIR([x264.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC([gcc clang])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
