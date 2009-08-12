#ifndef __SOCKS_H__
#define __SOCKS_H__

extern int _conf_use_socks;
extern int _conf_socks_proto;
extern int _conf_socks_port;
extern int _conf_socks_dns;
extern char *_conf_socks_username;
extern char *_conf_socks_host;
extern char *_conf_socks_password;

int socksify(char *, int);
int socks5_connect(char *, int);
int sendrecv(int, char *, int, char *, int);

#endif
