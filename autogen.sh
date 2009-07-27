#!/bin/sh

/opt/local/bin/autoheader
/opt/local/bin/aclocal
/opt/local/bin/automake -a
/opt/local/bin/autoconf
