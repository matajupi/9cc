int main() {
    int s = neko();
    one(s);
    return 0;
}

int neko() {
    return add() + add();
}

int add() {
    return 3;
}