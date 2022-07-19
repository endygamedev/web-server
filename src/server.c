#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include "server.h"
#include "request.h"

#define WEBSITE_FOLDER "/home/egor/Code/C/web-server/tests"


struct server server_init(int domain, int type, int protocol, unsigned int interface,
                          int port, int backlog)
{
    struct server serv;

    serv.domain = domain;
    serv.type = type;
    serv.protocol = protocol;
    serv.port = port;
    serv.backlog = backlog;
    
    serv.addr.sin_family = domain;
    serv.addr.sin_port = htons(port);
    serv.addr.sin_addr.s_addr = htonl(interface);

    serv.socket = socket(domain, type, protocol);
    
    if (bind(serv.socket, (struct sockaddr*)&serv.addr, sizeof(serv.addr)) < 0) {
        fprintf(stderr, "Error: Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    if (listen(serv.socket, serv.backlog) < 0) {
        fprintf(stderr, "Error: Listen failed\n");
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "Server is listening...\n\n");
    }

    return serv;
}


void launch(struct server serv)
{
    char buffer[BUFSIZ];
    int address_length = sizeof(serv.addr);
    FILE *logfile = fopen("/var/log/webserver.log", "w");

    if (!logfile) {
        fprintf(stderr, "Error: Can't open logfile\n");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        char *path = malloc(BUFSIZ);
        strcpy(path, WEBSITE_FOLDER);
        int accept_sockfd = accept(serv.socket, (struct sockaddr*)&serv.addr,
                                   (socklen_t *)&address_length);
        read(accept_sockfd, buffer, BUFSIZ);
        fprintf(logfile, "%s\n", buffer);
        
        parse_request(&path, buffer);

        if (file_exists(path)) {
            send_response(accept_sockfd, path);
        } else {
            strcpy(path, WEBSITE_FOLDER);
            strcat(path, "/404.html");
            send_response(accept_sockfd, path);
        }

        free(path);
        close(accept_sockfd);
        fflush(logfile);
    }

    fclose(logfile);
}
