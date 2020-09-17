/* Print the 10th Fibonacci number. */

# include <iostream>

using namespace std;

int fib(int n) {
    return n == 0 || n == 1 ? 1 : fib(n - 1) + fib(n - 2);
}

int main() {
    cout << fib(10) << endl;
}
