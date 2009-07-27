#!/usr/bin/env python

import sys

print "<INITIAL>",

for letter in sys.argv[1]:
    L = "[" + letter.upper() + letter.lower() + "]"
    sys.stdout.write(L)

print "\t\t\t{BEGIN _" + sys.argv[1].upper() + "; return " + sys.argv[1].upper() + ";}"

