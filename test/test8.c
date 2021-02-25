main() {
    s = neko();
    one(s);
}

neko() {
    return add() + add();
}

add() {
    return 3;
}