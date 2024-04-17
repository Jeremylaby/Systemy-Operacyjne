CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:clean
	$(CXX) $(CFLAGS) -o zad1 main.c -lm
	$(CXX) $(CFLAGS) -o zad2 main2.c
zad2:clean
	$(CXX) $(CFLAGS) -o zad2 main2.c
	$(CXX) $(CFLAGS) -o calculate calculate.c -lm
clean:
	rm -f zad1 reciever reciver sender main zad1 zad2 calculate