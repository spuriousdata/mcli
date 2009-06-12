bin_PROGRAMS = mci
BUILT_SOURCES = parser.h
CLEANFILES = lexer.c parser.c parser.h parser.output
AM_YFLAGS = -d
mci_SOURCES = mci.c rl_complete.c connection.c parser.y lexer.l socks.c configure.c
mci_LDADD = @LEXLIB@ 
EXTRA_DIST = mci.h rl_complete.h connection.h socks.h mci.conf-sample configure.h

if DEBUG
CFLAGS += -DDEBUG
AM_YFLAGS += --debug
endif
