#pragma once

int file_exists(char *path);
int is_folder(char *path, char *path_request);
void parse_request(char **path, char *buffer);
char *mime_type(char *extension);
void send_response(int sockfd, char *filename);
