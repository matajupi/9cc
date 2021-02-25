main() {
    for (n = 0; 10 > n; n = n + 1) {
        judge(n);
    }
}

judge(n) {
    if (n - ((n / 2) * 2) == 0) one(n);
    else {
        s = n + 1;
        one(s);
    }
}