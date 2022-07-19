#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>

#include "server.h"


int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Error: Not enough arguments were entered\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int backlog = atoi(argv[2]);

    if (!(port && backlog)) {
        fprintf(stderr, "Error: Invalid parameters\n");
        exit(EXIT_FAILURE);
    }

    struct server serv = server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, backlog);
    launch(serv);
    close(serv.socket);
}
