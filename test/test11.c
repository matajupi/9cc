int main() {
    int x = 3;
    int y = 5;
    int *z;
    z = &y + 8;
    one(*z);
    return 0;
}