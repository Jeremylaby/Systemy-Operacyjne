CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:clean
	$(CXX) $(CFLAGS) -o server serwer.c
	$(CXX) $(CFLAGS) -o client client.c
	$(CXX) $(CFLAGS) -o main main.c

clean:
	rm -f main server client