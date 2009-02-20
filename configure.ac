#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(mci, 0.0.1)
AM_INIT_AUTOMAKE([foreign]) dnl 'foreign' says not to check for gnu-package-standards (requiring NEWS, README, ChangeLog, etc)
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB
VL_LIB_READLINE  

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Set up custom flags
AC_ARG_ENABLE([debug],
[  --enable-debug    Turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gethostbyname memset socket strdup strstr strerror strtol])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
