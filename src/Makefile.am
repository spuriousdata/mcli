bin_PROGRAMS = mci
BUILT_SOURCES = parser.h
CLEANFILES = lexer.c parser.c parser.h parser.output
AM_YFLAGS = -d
mci_SOURCES = mci.c rl_complete.c connection.c parser.y lexer.l socks.c configure.c pgrep.c
mci_LDADD = @LEXLIB@ @PCRE_LIBS@
EXTRA_DIST = mci.h rl_complete.h connection.h socks.h mci.conf-sample configure.h pgrep.h

if DEBUG
CFLAGS += -DDEBUG -ggdb
AM_CFLAGS = -O0
AM_YFLAGS += --debug
endif

if USE_PCRE
CFLAGS += -DPCRE @PCRE_CFLAGS@
endif
