CC = gcc
CFLAGS = -Wall -Wextra

static_library:
	$(CC) $(CFLAGS) -c collatz.c
	ar rcs libcollatz.a collatz.o
	$(CC) $(CFLAGS) -c main.c
	$(CC) $(CFLAGS) main.o -l collatz -L ./ -o main_static
	./main_static

shared_library:
	$(CC) $(CFLAGS) -fPIC -c collatz.c -o collatz.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,libcollatz.so.1 -o libcollatz.so.1.0.1 collatz.o -lc
	ln -s libcollatz.so.1.0.1 libcollatz.so.1
	ln -s libcollatz.so.1 libcollatz.so
	$(CC) $(CFLAGS) main.c -lcollatz -L./ -Wl,-rpath,. -o main_shared
	./main_shared
dynamic_library:
	$(CC) $(CFLAGS) -DMAX_ITER=20 -fPIC -shared -o libcollatz.so collatz.c
	$(CC) $(CFLAGS) -o main_dynamic main_dynamic.c -ldl
	./main_dynamic
.PHONY: clean
clean:
	rm -f libcollatz.so.1 libcollatz.so.1.0.1
	rm -f *.o *.a *.so main_static main main_shared libcollatz main_dynamic