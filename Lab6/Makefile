CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:clean
	$(CXX) $(CFLAGS) -o zad1 main.c -lm
	$(CXX) $(CFLAGS) -o zad2 main2.c
	$(CXX) $(CFLAGS) -o calculate calculate.c -lm
zad1:clean
	$(CXX) $(CFLAGS) -o zad1 main.c -lm
zad2:clean
	$(CXX) $(CFLAGS) -o zad2 main2.c
	$(CXX) $(CFLAGS) -o calculate calculate.c -lm
example1:zad1
	./zad1 0.00001 3
example2:zad2
	./zad2 0.0 0.1
clean:
	rm -f zad1 reciever reciver sender main zad1 zad2 calculate pipe