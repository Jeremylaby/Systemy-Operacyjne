CC = gcc
CFLAGS = -Wall -Wextra
reverse:
	$(CC) $(CFLAGS) -c reverse.c
	ar rcs libreverse.a reverse.o
build_main:clean
	make reverse
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) main.o -l reverse -L ./ -o main
start_main:build_main
	./main example.txt output.txt
#test_reverse:
#	$(CC) $(CFLAGS) -o reverse reverse.c
#	./reverse example.txt tmp1.txt
#	./reverse tmp1.txt tmp2.txt
#	diff -s tmp2.txt example.txt > diff.txt
build_filessize:clean
	$(CC) $(CFLAGS) -o filessize filessize.c
start_filessize:build_filessize
	./filessize
all:clean
	make reverse
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) main.o -l reverse -L ./ -o main
	$(CC) $(CFLAGS) -o filessize filessize.c
test:
	./main example.txt output.txt
	./filessize
clean:
	rm -f *.o *.a *.so main reverse filessize