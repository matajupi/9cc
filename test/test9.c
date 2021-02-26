int main() {
    int n = fib(3);
    one(n);
    return 0;
}

int fib(int n) {
    if (n < 2) return 1;
    return fib(n - 1) + fib(n - 2);
}