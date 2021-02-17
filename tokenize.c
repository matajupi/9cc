#include "9cc.h"

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
static bool startswith(char *p, char *q);

// Report error and exit
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// Reports an error location and exit
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " "); // print pos space
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

static bool startswith(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

static Token *new_token(TokenKind kind, Token *cur, char *str, int len);

// 新しいトークンを作成してcurにつなげる
static Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

void tokenize(char *p);
static int is_alnum(char c);

// Tokenize 1line and return linking list
void tokenize(char *p) {
    Token head; // 先頭のトークン
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // Skip space
        if (isspace(*p)) {
            p++;
            continue;
        }
        // Multi-letter punctuator
        if (startswith(p, "==") || startswith(p, "!=") ||
            startswith(p, "<=") || startswith(p, ">=")) {
                cur = new_token(TK_RESERVED, cur, p, 2);
                p += 2;
                continue;
        }
        // Single-letter punctuator
        if (strchr("+-*/()<>;={}", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }
        // if statement
        if (startswith(p, "if") && !is_alnum(p[2])) {
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
            continue;
        }
        // else statement
        if (startswith(p, "else") && !is_alnum(p[4])) {
            cur = new_token(TK_ELSE, cur, p, 4);
            p += 4;
            continue;
        }
        // while statement
        if (startswith(p, "while") && !is_alnum(p[5])) {
            cur = new_token(TK_WHILE, cur, p, 5);
            p += 5;
            continue;
        }
        // for statement
        if (startswith(p, "for") && !is_alnum(p[3])) {
            cur = new_token(TK_FOR, cur, p, 3);
            p += 3;
            continue;
        }
        // return keyword
        if (startswith(p, "return") && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }
        // Integer literal
        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            // Judge number length at strtol and update p and seek number length
            cur->len = p - q;
            continue;
        }
        // Identifier (OK)
        if (is_alnum(*p)) {
            int len = 1;
            cur = new_token(TK_IDENT, cur, p++, len);
            for (; is_alnum(*p); p++) len++;
            cur->len = len;
            continue;
        }

        error_at(p, "トークナイズできません");
    }
    // End of file
    new_token(TK_EOF, cur, p, 0);
    token = head.next;
}

static int is_alnum(char c) {
    return ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            (c == '_');
}