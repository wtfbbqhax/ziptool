AC_PREREQ(2.61)
AC_INIT([ziptool], [2.1.0], [http://code.google.com/p/ziptool/issues/])
AC_CONFIG_SRCDIR([src/ziptool.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
LT_INIT
AC_PROG_LIBTOOL

case "$host" in
    *-darwin*)
    AC_DEFINE([DARWIN], [1], [Define if MacOS])
    ;;

    *-bsd*)
    AC_DEFINE([BSD], [1], [Define if BSD])
    ;;

    *-linux*)
    AC_DEFINE([LINUX], [1], [Define if Linux])
    ;;
esac

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_TYPE_SIGNAL
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([inet_ntoa memset strdup strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
