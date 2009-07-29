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

########
# PCRE #
########
AC_ARG_WITH([pcre],
			[AS_HELP_STRING([--with-pcre@<:@=ARG@:>@], [enable support for PCRE library, optionally specify the full path (default use pcre if found)])],
			[
			 if test "$withval" = "no" ; then
				 want_pcre="no"
			 else
				 want_pcre="yes"
				 require_pcre="yes"
				 if test "$withval" != "yes" ; then
					 if test ! -d "$withval" ; then
						 AC_MSG_ERROR([$withval is not a directory])
					 fi
					 PCRE_HOME="$withval"
				 fi 
			 fi
			],
			[want_pcre="yes" require_pcre="no"]
)

PCRE_LIBS=""
PCRE_CFLAGS=""

if test "$want_pcre" = "no" ; then
	AC_MSG_NOTICE([ignoring PCRE (optional)])
else
	if test -z "$PCRE_HOME" ; then
		AC_CHECK_LIB([pcre], [pcre_compile], [HAVE_PCRE="YES"], [])
		if test "$HAVE_PCRE" = "yes" ; then
			PCRE_LIBS="-lpcre"
		fi
	else
		PCRE_OLD_LDFLAGS="$LDFLAGS" ; LDFLAGS="$LDFLAGS -L$PCRE_HOME/lib"
		PCRE_OLD_CPPFLAGS="$CPPFLAGS" ; CPPFLAGS="$CPPFLAGS -I$PCRE_HOME/include"
		AC_CHECK_LIB(pcre, pcre_compile, [HAVE_PCRE="yes"], [])
		CPPFLAGS="$PCRE_OLD_CPPFLAGS"
		LDFLAGS="$PCRE_OLD_LDFLAGS"
		if test "$HAVE_PCRE" = "yes" ; then
			PCRE_LIBS="-L$PCRE_HOME/lib -lpcre"
			test -d "$PCRE_HOME/include" && PCRE_CFLAGS="-I$PCRE_HOME/include"
		fi
	fi
	AC_MSG_CHECKING([PCRE library (optional)])
	if test "$HAVE_PCRE" = "yes" ; then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		if test "$require_pcre" = "yes" ; then
			AC_MSG_ERROR([PCRE not found or incompatible (requested)])
		else
			AC_MSG_NOTICE([disabling PCRE (optional)])
		fi
	fi
fi

AM_CONDITIONAL([USE_PCRE], [test x$HAVE_PCRE = xyes])
AC_SUBST([HAVE_PCRE])
AC_SUBST([PCRE_LIBS])
AC_SUBST([PCRE_CFLAGS])


# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gethostbyname memset socket strdup strstr strerror strtol getopt memmove strchr])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

