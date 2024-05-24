CXX = gcc
CFLAGS = -Wall -Wextra
all:clean
	$(CXX) $(CFLAGS) -o zad1 main.c
	$(CXX) $(CFLAGS) -o zad2 zad2.c
	$(CXX) $(CFLAGS) -o zad3 zad3.c
	$(CXX) $(CFLAGS) -o zad_dom1 zad_dom1.c
	$(CXX) $(CFLAGS) -o catcher catcher.c
	$(CXX) $(CFLAGS) -o sender sender.c
zad_dom1:clean
	$(CXX) $(CFLAGS) -o zad_dom1 zad_dom1.c
zad_dom1_example:zad_dom1
	./zad_dom1 wrongArg
	./zad_dom1 handler
	./zad_dom1 mask
	./zad_dom1 ignore
	./zad_dom1 none
zad_dom2:clean
	$(CXX) $(CFLAGS) -o catcher catcher.c
	$(CXX) $(CFLAGS) -o sender sender.c

clean:
	rm -f zad1 zad2 zad3 zad_dom1 sender catcher main