bin_PROGRAMS = mci
BUILT_SOURCES = parser.h
CLEANFILES = lexer.c parser.c parser.h parser.output
AM_YFLAGS = -d
AM_CFLAGS = @PCRE_CFLAGS@
mci_SOURCES = mci.c rl_complete.c connection.c parser.y lexer.l socks.c configure.c pgrep.c util.c
mci_LDADD = @LEXLIB@ @PCRE_LIBS@
EXTRA_DIST = mci.h rl_complete.h connection.h socks.h mci.conf-sample configure.h pgrep.h util.h

if DEBUG
CFLAGS += -DDEBUG -ggdb
AM_CFLAGS += -O0
#AM_YFLAGS += --debug
endif
