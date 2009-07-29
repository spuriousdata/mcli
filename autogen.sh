#!/bin/sh

autoheader
aclocal -I /usr/local/share/aclocal
automake -a
autoconf
