#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type of abstract syntax tree node
// >, >= は <, <= の左右の項を入れ替える
typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_ASSIGN, // =
    ND_RETURN, // return keyword
    ND_IF,     // if keyword
    ND_LVAR, // Lcoal variable
    ND_NUM, // Integer
} NodeKind;

typedef struct Node Node;

// Abstract syntax tree node
struct Node {
    NodeKind kind; // node type
    Node *lhs;     // left side
    Node *rhs;     // right side
    int val;       // Use only when kind is ND_NUM
    int offset;    // Use only when kind is ND_LVAR
};

// Type of token
typedef enum {
    TK_RESERVED, // Symbol
    TK_IDENT,    // Identifier
    TK_RETURN,   // return keyword
    TK_IF,       // if keyword
    TK_NUM,      // Number
    TK_EOF,      // End of file
} TokenKind;

typedef struct Token Token;

// Token
struct Token {
    TokenKind kind; // トークンのタイプ
    Token *next;    // 連結リストにおいての次の入力トークン
    int val;        // kindがTK_NUMの場合その数値
    char *str;      // トークン文字列
    int len;        // Length of token
};

typedef struct LVar LVar;

// Type of Local variable.
struct LVar {
    LVar *next; // Next variable or NULL
    char *name; // Name
    int len;    // Length of name
    int offset; //Offset from RBP
};

// Local variable.
extern LVar *locals;

// Input program
extern char *user_input;

// Current token
extern Token *token;

// code
extern Node *code[];

// Reports error and exit
extern void error(char *fmt, ...);

// Reports error with pointer and exit
extern void error_at(char *loc, char *fmt, ...);

// Tokenize
extern void tokenize(char *p);

// Get expression node in current token
extern void program();

// Generate assembly from node
extern void gen(Node *node);
