#include "hostentry.h"

QRegExp HostEntry::port_regex = QRegExp("[1-9][0-9]{0,4}");

HostEntry::~HostEntry()
{
    delete host;
    delete port;
}
