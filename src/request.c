#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>

#include "request.h"


int file_exists(char *path)
{
    struct stat s = {0};
    return !(stat(path, &s));
}


void parse_request(char **path, char *buffer)
{
    char *request = strtok(buffer, "\n");
    char *path_request = strtok(request, " ");
    path_request = strtok(NULL, " ");
    
    if (!strcmp(path_request, "/")) {
        char filename[BUFSIZ];
        strcpy(filename, "/index.html");
        strcat(*path, filename);
    } else {
        strcat(*path, path_request);
    }
}


char *mime_type(char *extension)
{
    if (!(strcmp(extension, "html"))) {
        return "text/html";
    } else if (!(strcmp(extension, "css"))) {
        return "text/css";
    } else if (!(strcmp(extension, "ico"))) {
        return "image/x-icon";
    } else {
        return "";
    }
}


void send_response(int sockfd, char *path)
{
    long length;
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
    
    char *iter = strtok(path, ".");
    char *token = iter;

    while ((iter = strtok(NULL, ".")) != NULL) {
        token = iter;
    }

    char *type = malloc(BUFSIZ);
    type = mime_type(token);

    if (!strcmp(type, "image/x-icon")) {
        strcat(path, ".ico");
        int fdicon = open(path, O_RDONLY);
        struct stat buffer;
        stat(path, &buffer);
        sendfile(sockfd, fdicon, NULL, buffer.st_size);
        close(fdicon);
    } else {
        char *response = malloc(length + BUFSIZ);
        strcpy(response, "HTTP/1.1 200 OK\nConnection: close\nContent-Type: ");
        strcat(response, type);
        strcat(response, "\n\n");
        strcat(response, content);
        write(sockfd, response, strlen(response));
        free(response);
    }
}
