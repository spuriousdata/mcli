#ifndef HOSTENTRY_H
#define HOSTENTRY_H

#include <QLineEdit>
class QRegExp;

class HostEntry
{
public:
    HostEntry() {}
    ~HostEntry();
    QLineEdit *host;
    QLineEdit *port;
    static QRegExp port_regex;
};

#endif // HOSTENTRY_H
