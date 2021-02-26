int main() {
    int n = 3;
    int m = 4;
    neko(n, m);
    n = 4;
    m = 3;
    neko(n, m);
    return 0;
}

int neko(int n, int m) { 
    one(n + m);
    return 0;
}
