#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "request.h"

#define WEBSITE_FOLDER "/home/egor/Code/C/web-server/tests/"


void send_response(int sockfd, char *filename)
{
    char path[BUFSIZ];
    long length;
    
    strcpy(path, WEBSITE_FOLDER);
    strcat(path, filename);
    FILE *file = fopen(path, "r");

    if (!file) {
        fprintf(stderr, "Error: File can't be opened\n");
        exit(EXIT_FAILURE);
    }
    
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *content = malloc(length);
    
    if (content) {
        fread(content, 1, length, file);
    }
    
    fclose(file);
    
    char *iter = strtok(filename, ".");
    char *token = iter;

    while ((iter = strtok(NULL, ".")) != NULL) {
        token = iter;
    }
    
    char *response = malloc(length + BUFSIZ);
    strcpy(response, "HTTP/1.1 200 OK\nX-Content-Type-Options: nosniff\nContent-Type: text/");
    strcat(response, token);
    strcat(response, "\n\n");
    strcat(response, content);
    printf("%s\n\n", response);
    fflush(stdout);
    write(sockfd, response, strlen(response));
}
