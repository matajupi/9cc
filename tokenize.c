#include "9cc.h"

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

// 新しいトークンを作成してcurにつなげる
static Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

// Tokenize 1line and return linking list
Token *tokenize(char *p) {
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
        if (strchr("+-*/()<>", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
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

        error_at(p, "トークナイズできません");
    }
    // End of file
    new_token(TK_EOF, cur, p, 0);
    return head.next;
}