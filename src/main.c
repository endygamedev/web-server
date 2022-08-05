#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#include <netinet/ip.h>

#include "server.h"


int main(int argc, char *argv[])
{

    char *in_port;
    char *in_backlog;
    int c;

    while ((c = getopt(argc, argv, "p:b:")) != -1) {
        switch (c) {
            case 'p':
                in_port = optarg;
                break;
            case 'b':
                in_backlog = optarg;
                break;
            case '?':
                if (optopt == 'p' || optopt == 'b') {
                    fprintf(stderr, "Error: Option [-%c] requires an argument\n",
                                                                        optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Error: Unknown option [-%c]\n", optopt);
                } else {
                    fprintf(stderr, "Error: Unknown option character [-\\x%x]\n",
                                                                        optopt);
                }
                exit(EXIT_FAILURE);
            default:
                abort();
        }
    }

    if (!(in_port && in_backlog)) {
        fprintf(stderr, "Usage: main -p <port> -b <backlog>\n");
        exit(EXIT_FAILURE);
    }

    int port = atoi(in_port);
    int backlog = atoi(in_backlog);

    if (!(port && backlog)) {
        fprintf(stderr, "Error: Invalid parameters\n");
        exit(EXIT_FAILURE);
    }

    for (int i = optind; i < argc; i++) {
        fprintf(stderr, "Warning: Non-option argumnet `%s`\n", argv[i]);
    }
    
    /* Initialize server */
    struct server serv = server_init(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, backlog);
    
    /* Launch server */
    launch(serv);

    /* Close server socket */
    close(serv.socket);

    return EXIT_SUCCESS;
}
