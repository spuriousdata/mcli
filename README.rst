=============================
Memcache CLI
=============================

*Memcache CLI* is a tool that I wrote to aid in debugging of websites utilizing
memcache.  The current version of mcli (0.0.3) utilizes readline for advanced
command line editing and can link against libreadline, libedit, or libeditline.
New in version 0.0.3 is pcre_ support for grep-like functionality. Running a
command such as::

    memcache> stats | grep connect
    STAT curr_connections 3
    STAT total_connections 75
    STAT connection_structures 4

will 'grep' through the output of the stats command for the string 'connect'
and print only lines that match.

The command-line will send anything that is typed at the prompt (with a few
exceptions) to the memcache server(s). Currently supported memcache commmands
are listed in the readline-tab-completion list and all commands are added to
your current session's readline history.

The command-line version also supports binding commands to a specific server
with the ''with'' command:::

    memcache> list
    0: memcache.local:11211
    1: memcache2.local:11211

    memcache> with 0
    active server is now 0: memcache.local:11211

commands will now be sent only to memcache server 0.

To unbind:::

    memcache> with -1
    active server is now -1: All
 
.. _pcre: http://pcre.org
