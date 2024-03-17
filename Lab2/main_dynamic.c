#include <stdio.h>
#include <dlfcn.h>
#ifndef MAX_ITER
#define MAX_ITER 50
#endif

int main() {
    void *handle = dlopen("./libcollatz.so", RTLD_LAZY);
    if(!handle){/*error*/}

    int (*test_collatz_convergence)(int, int) = dlsym(handle, "test_collatz_convergence");
    if(dlerror() != NULL){/*error*/}
    printf("input: 4 max_iter: %d result: %d\n",MAX_ITER, test_collatz_convergence(4, 10));
    printf("input: 3 max_iter: %d result: %d\n",MAX_ITER, test_collatz_convergence(3, 30));
    printf("input: 10 max_iter: %d result: %d\n",MAX_ITER, test_collatz_convergence(10, 50));
    printf("input: 23 max_iter: %d result: %d\n",MAX_ITER, test_collatz_convergence(23, 60));

    dlclose(handle);
    return 0;
}
//
// Created by stani on 12.03.2024.
//
