#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(mci, 0.0.2)
AM_INIT_AUTOMAKE([foreign]) dnl 'foreign' says not to check for gnu-package-standards (requiring NEWS, README, ChangeLog, etc)
AC_CONFIG_SRCDIR([src/mci.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_RANLIB
VL_LIB_READLINE
AX_PATH_LIB_PCRE

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Set up custom flags
AC_ARG_ENABLE([debug],
			  [AS_HELP_STRING([--enable-debug], [Turn on debugging])],
			  [enable_debug=true])
AM_CONDITIONAL([DEBUG], [test x$enable_debug = xtrue])

AC_ARG_WITH([readline],
			[AS_HELP_STRING([--without-readline], [disable support for readline])],
			[],
			[with_readline=yes])
AM_CONDITIONAL([READLINE], [test x$with_readline = xyes])

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gethostbyname memset socket strdup strstr strerror strtol getopt memmove strchr])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

