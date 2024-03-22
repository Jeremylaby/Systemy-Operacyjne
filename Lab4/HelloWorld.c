//
// Created by stani on 22.03.2024.
//
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    printf("Hello World %d\n",(int)getpid());
}