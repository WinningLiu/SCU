/* Print the 10th Fibonacci number. */

class fib {
    static int fib(int n) {
	return n == 0 || n == 1 ? 1 : fib(n - 1) + fib(n - 2);
    }

    public static void main(String[] args) {
	System.out.println(fib(10));
    }
}
