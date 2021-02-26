int main() {
    for (int n = 0; 10 > n; n = n + 1) {
        judge(n);
    }
    return 0;
}

int judge(int n) {
    if (n - ((n / 2) * 2) == 0) one(n);
    else {
        int s = n + 1;
        one(s);
    }
    return 0;
}