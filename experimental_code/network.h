#ifndef PVN_NETWORK_H
#define PVN_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFSIZE 255

inline void Die(char *mess) { perror(mess); exit(1); }

#endif // PVN_NETWORK_H
