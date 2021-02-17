#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 "0;"
assert 42 "42;"

assert 21 "5+20-4;"
assert 41 "12 + 34 - 5;"
assert 47 "5+6*7;"
assert 17 "4 * 2 + 3 * 3;"
assert 15 "5 * (9 - 6);"
assert 4 "(3 + 5) / 2;"
assert 10 "- -10;"
assert 10 "- - +10;"

assert 0 "0 == 1;"
assert 1 "42 == 42;"
assert 1 "0 != 1;"
assert 0 "42 != 42;"

assert 1 "0 < 1;"
assert 0 "1 < 1;"
assert 0 "2 < 1;"
assert 1 "0 <= 1;"
assert 1 "1 <= 1;"
assert 0 "2 <= 1;"

assert 1 "1 > 0;"
assert 0 "1 > 1;"
assert 0 "1 > 2;"
assert 1 "1 >= 0;"
assert 1 "1 >= 1;"
assert 0 "1 >= 2;"

assert 3 "a=3;"
assert 5 "b=5;"
assert 10 "b = 5; c = b * 2;"
assert 100 "b = 10; c = b * 100; d = c / b;"

assert 10 "num = 10;"
assert 20 "num = 10; ans = num * 2;"
assert 90 "num = 100; sub = 10; ans = num - sub;"
assert 100 "num = 10; num = num * 10;"

assert 10 "return 10;"
assert 100 "num = 10; mul = 10; return num * mul;"
assert 100 "return 10 * 10;"

assert 10 "if (1) return 10; return 3;"
assert 10 "if (0) return 9; return 10;"
assert 1 "num = 10; if (num == 10) return 1; return 3;"

assert 10 "if (0) return 1; else return 10;"
assert 10 "num = 10; if (num == 3) return 3; else if (num == 10) return 10; return 0;"
assert 10 "num = 10; if (num == 3) return 3; else if (num == 4) return 4; else if (num == 5) return 5; else return num;"
assert 10 "num = 10; if (num > 0) if (num == 1) return 1; else return 10;"
assert 10 "num = 10; if (num > 0) if (num == 10) return 10;"

assert 10 "p = 0; while (10 > p) p = p + 1; return p;"
assert 10 "p = 0; while (1) if (p == 10) return p; else p = p + 1;"

assert 10 "p = 0; for (i = 0; 10 > i; i = i + 1) p = p + 1; return p;"
assert 100 "p = 0; for (i = 0; 10 > i; i = i + 1) for (j = 0; 10 > j; j = j + 1) p = p + 1; return p;"
assert 10 "p = 0; for (i = 100; 0 < i; i = i - 1) if (i == 90) return p; else p = p + 1;"

assert 10 "p = 0; for (i = 0; 10 > i; i = i + 1) { p = p + 1; } return p;"
assert 20 "p = 0; for (i = 0; 10 > i; i = i + 1) { p = p + 1; p = p + 1; } return p;"
assert 10 "p = 9; if (p == 9) { p = p + 1; return p; } return 0;"
assert 10 "p = 0; while (p != 10) { p = p + 1; } return p;"

echo OK
