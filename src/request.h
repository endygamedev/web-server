#pragma once

int file_exists(char *path);
void parse_request(char **path, char *buffer);
char *mime_type(char *extension);
void send_response(int sockfd, char *filename);
