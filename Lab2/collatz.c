//
// Created by stani on 12.03.2024.
//
#include "stdio.h"
int collatz_conjecture(int input){
    if(input%2==0){
      return input/2;
    }else{
        return input*3;
    }
}
int test_collatz_convergence(int input, int max_iter){
    int x = input;
    if(x<=1){
        printf("Wrong input\n");
        return -1;
    }
    for(int i=0;i<max_iter;i++){
        x= collatz_conjecture(x);
        if(x==1){
            return i+1;
        }
    }
    return -1;
}