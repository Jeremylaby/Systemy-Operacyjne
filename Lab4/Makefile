CXX = gcc
CFLAGS = -Wall -Wextra
.PHONY: all countdown clean
all:clean
	$(CXX) $(CFLAGS) -o zad1 main.c
	$(CXX) $(CFLAGS) -o zad2 main2.c
zad1:clean
	$(CXX) $(CFLAGS) -o zad1 main.c
zad2:clean
	$(CXX) $(CFLAGS) -o zad2 main2.c
additional:clean
	$(CXX) $(CFLAGS) -o helloworld helloworld.c
	$(CXX) $(CFLAGS) -o parent parent.c
	$(CXX) $(CFLAGS) -o exercise nextexercise.c
example1:zad1
	@echo "First example of 5 processes"
	./zad1 5
	@echo "Second example of 30 processes"
	./zad1 30
example2:zad2
	@echo "First example correct path"
	./zad2 cmake-build-debug
	@echo "Second example incorrect path"
	./zad2 wfgqg

clean:
	rm -f helloworld main parent exercise zad1 zad2