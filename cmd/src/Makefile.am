bin_PROGRAMS = mci
BUILT_SOURCES = parser.h
CLEANFILES = lexer.c parser.cc parser.h parser.output
AM_YFLAGS = -d
AM_CFLAGS = @PCRE_CFLAGS@
AM_CXXFLAGS = @PCRE_CFLAGS@
mci_SOURCES = mci.cc rl_complete.c connection.cc parser.yy lexer.l socks.cc configure.c pgrep.cc util.cc Configuration.cc
mci_LDADD = @LEXLIB@ @PCRE_LIBS@
EXTRA_DIST = mci.h rl_complete.h connection.h socks.h mci.conf-sample configure.h pgrep.h util.h hostent.h Configuration.h

if DEBUG
CFLAGS += -DDEBUG -ggdb
CXXFLAGS += -DDEBUG -ggdb
AM_CFLAGS += -O0
AM_CXXFLAGS += -O0
AM_YFLAGS += --debug
endif
