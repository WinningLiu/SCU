/* Print the 10th Fibonacci number */

# include <stdio.h>

int fib(int n) {
    return n == 0 || n == 1 ? 1 : fib(n - 1) + fib(n - 2);
}

int main(void) {
    printf("%d\n", fib(10));
}
