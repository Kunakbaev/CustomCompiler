int fact(int n) {
    if (n <= 1) return 1;
    return fact(n - 1) * n;
}

int main() {
    int arg;
    input(arg);
    output(fact(arg));

    return 0;
}
