bin_PROGRAMS = mci
BUILT_SOURCES = parser.hh
CLEANFILES = lexer.c parser.cc parser.hh parser.output
#AM_YFLAGS = -d
AM_YFLAGS = -d --debug
AM_CFLAGS = @PCRE_CFLAGS@
AM_CXXFLAGS = @PCRE_CFLAGS@
mci_SOURCES = mci.cc rl_complete.c Connection.cc parser.yy lexer.l pgrep.cc util.cc Configuration.cc HostEnt.h MemcacheClient.cc ConnectionReader.cc InputInterpreter.cc
mci_LDADD = @LEXLIB@ @PCRE_LIBS@ -lpthread
EXTRA_DIST = mci.h rl_complete.h Connection.h mci.conf-sample pgrep.h util.h Configuration.h MemcacheClient.h ConnectionReader.h InputInterpreter.h

if DEBUG
CFLAGS += -DDEBUG -ggdb
CXXFLAGS += -DDEBUG -ggdb
AM_CFLAGS += -O0
AM_CXXFLAGS += -O0
endif
