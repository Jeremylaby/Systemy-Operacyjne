#include <stdio.h>

int test_collatz_convergence(int input, int max_iter);
int main() {
    printf("input: 4 max_iter: 10 result: %d\n",test_collatz_convergence(4,10));
    printf("input: 3 max_iter: 30 result: %d\n",test_collatz_convergence(3,30));
    printf("input: 10 max_iter: 50 result: %d\n",test_collatz_convergence(10,50));
    printf("input: 23 max_iter: 60 result: %d\n",test_collatz_convergence(23,60));
    return 0;
}
