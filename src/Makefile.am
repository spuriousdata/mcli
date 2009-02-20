bin_PROGRAMS = mci
BUILT_SOURCES = parser.h
CLEANFILES = lexer.c parser.c parser.h parser.output
AM_YFLAGS = -d
mci_SOURCES = main.c rl_complete.c parser.y lexer.l socks.c
mci_LDADD = @LEXLIB@ 
EXTRA_DIST = host.h mci.conf-sample

if DEBUG
CFLAGS += -DDEBUG
AM_YFLAGS += --debug
endif
