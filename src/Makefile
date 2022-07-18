SERVER=server
REQUEST=request

CC_FLAGS=-std=gnu99 -Wall -Werror -Wpedantic -Wextra
CC=gcc

all: $(SERVER).c $(REQUEST).c
	$(CC) $(REQUEST).c $(SERVER).c main.c -o main $(CC_FLAGS)

server: $(REQUEST).c
	$(CC) $(REQUEST).c $(SERVER).c -o $(SERVER) $(CC_FLAGS)

request:
	$(CC) $(REQUEST).c -o $(REQUEST) $(CC_FLAGS)


clean:
	rm -f main
	rm -f $(SERVER)
	rm -f $(REQUEST)
