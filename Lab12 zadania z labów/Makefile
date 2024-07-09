CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:clean
	make server
	make client
main:clean
	$(CXX) $(CFLAGS) -o main main.c
server:
	$(CXX) $(CFLAGS) -o server server.c
client:
	$(CXX) $(CFLAGS) -o client client.c
clean:
	rm -f main server client