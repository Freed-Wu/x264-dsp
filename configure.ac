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

AC_ARG_WITH([x264-bit-depth],
            [AS_HELP_STRING([--with-x264-bit-depth@<:@=8|10@:>@],
                            [bit depth, default=8]
                            )
            ],
            [],
            [with_x264_bit_depth=yes]
            )
AS_IF([test "x$with_x264_bit_depth" = "xyes"],
      [with_x264_bit_depth=8],
      []
    )
AS_IF([test "x$with_x264_bit_depth" = "xno"],
      [AC_MSG_ERROR([must define X264_BIT_DEPTH!])],
      []
    )
AS_IF([test "x$with_x264_bit_depth" = x8 -o "x$with_x264_bit_depth" = x10],
      [AC_MSG_NOTICE([define X264_BIT_DEPTH="$with_x264_bit_depth"])],
      [AC_MSG_ERROR(["$with_x264_bit_depth" is an illegal bit depth!])]
      )
AC_SUBST(with_x264_bit_depth)

AC_ARG_WITH([x264-chroma-format],
            [AS_HELP_STRING([--with-x264-chroma-format@<:@=0..3@:>@],
                            [chroma format, default=1]
                            )
            ],
            [],
            [with_x264_chroma_format=yes]
            )
AS_IF([test "x$with_x264_chroma_format" = "xyes"],
      [with_x264_chroma_format=1],
      []
    )
AS_IF([test "x$with_x264_chroma_format" = "xno"],
      [AC_MSG_ERROR([must define X264_CHROMA_FORMAT!])],
      []
    )
AS_IF([test "$with_x264_chroma_format" -le 3 -a "$with_x264_chroma_format" -ge 0],
      [AC_MSG_NOTICE([define X264_CHROMA_FORMAT="$with_x264_chroma_format"])],
      [AC_MSG_ERROR(["$with_x264_chroma_format" is an illegal chroma format!])]
      )
AC_SUBST(with_x264_chroma_format)

AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug],
                              [enable debug]
                              )
              ],
              [],
              [enable_debug=no]
              )
AS_IF([test "x$enable_debug" = xyes],
      [CFLAGS="$CFLAGS -ggdb -O0 -pedantic"],
      []
    )

# Checks for programs.
AC_PROG_CC([gcc clang])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
