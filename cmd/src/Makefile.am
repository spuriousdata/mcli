bin_PROGRAMS = mci
BUILT_SOURCES = parser.h
CLEANFILES = lexer.c parser.cc parser.h parser.output
AM_YFLAGS = -d
AM_CFLAGS = @PCRE_CFLAGS@
AM_CXXFLAGS = @PCRE_CFLAGS@
mci_SOURCES = mci.cc rl_complete.c Connection.cc parser.yy lexer.l pgrep.cc util.cc Configuration.cc HostEnt.h MemcacheClient.cc ConnectionReader.cc InputInterpreter.cc
mci_LDADD = @LEXLIB@ @PCRE_LIBS@
EXTRA_DIST = mci.h rl_complete.h Connection.h mci.conf-sample pgrep.h util.h Configuration.h MemcacheClient.h ConnectionReader.h InputInterpreter.h

if DEBUG
CFLAGS += -DDEBUG -ggdb
CXXFLAGS += -DDEBUG -ggdb
AM_CFLAGS += -O0
AM_CXXFLAGS += -O0
AM_YFLAGS += --debug
endif
