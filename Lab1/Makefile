CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:
	$(CXX) $(CFLAGS) -o main main.c
	$(CXX) $(CFLAGS) -o countdown countdown.c
	@echo "Siema Siema"
countdown:
	$(CXX) $(CFLAGS) -o countdown countdown.c
clean:
	rm -f countdown main