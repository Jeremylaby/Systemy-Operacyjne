CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:clean
	$(CXX) $(CFLAGS) -o main main.c
	$(CXX) $(CFLAGS) -o helloworld helloworld.c
	$(CXX) $(CFLAGS) -o parent parent.c
	$(CXX) $(CFLAGS) -o exercise nextexercise.c
clean:
	rm -f helloworld main parent