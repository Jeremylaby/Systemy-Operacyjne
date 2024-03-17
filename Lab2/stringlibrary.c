//
// Created by stani on 08.03.2024.
//
#include "stdio.h"
void printMessage(){
    printf("My very interesting library");
}
//Bibloteka współdzielona komendy
//gcc -fPIC -c stringlibrary.c -o stringlibrary.o
//gcc -shared -Wl,-soname,libstringlibrary.so.1 -o libstringlibrary.so.1.0.1 stringlibrary.o -lc
//ln -s libstringlibrary.so.1.0.1 libstringlibrary.so.1
//ln -s libstringlibrary.so.1 libstringlibrary.so
//gcc main.c -lstringlibrary -L./ -Wl,-rpath,. -o main
//./main