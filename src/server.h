#pragma once

#include <sys/socket.h>


struct server {
    int domain;
    int type;
    int protocol;
    unsigned int interface;
    int port;
    int backlog;
    struct sockaddr_in addr;
    int socket;
};

struct server server_init(int domain, int type, int protocol,
                          unsigned int interface, int port, int backlog);

void launch(struct server serv);
