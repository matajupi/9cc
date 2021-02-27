int main() {
    int x;
    int *y;
    x = 4;
    y = &x;
    *y = 3;
    one(*y);
    return 0;
}