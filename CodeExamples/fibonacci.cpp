int fibonacci(int n, int x) {
    if (n <= 2)
        return 1;

    return fibonacci(n - 1, x) + fibonacci(n - 2, x);
}

int main() {
    int arg;
    input(arg);
    int res = fibonacci(arg, 10);
    output(res);

    return 0;
}
