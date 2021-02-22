#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type of token
typedef enum {
    TK_RESERVED, // Symbol
    TK_IDENT,    // Identifier
    TK_RETURN,   // return keyword
    TK_IF,       // if keyword
    TK_ELSE,     // else keyword
    TK_WHILE,    // while keyword
    TK_FOR,      // for keyword
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

// Type of abstract syntax tree node
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
    ND_ELSE,   // else keyword
    ND_WHILE,  // while keyword
    ND_FOR,    // for keyword
    ND_LVAR, // Lcoal variable
    ND_FNCALL, // Function call
    ND_BLOCK, // Block
    ND_FNDEF,  // Function definition
    ND_NUM, // Integer
} NodeKind;

typedef struct Node Node;

// Abstract syntax tree node
struct Node {
    NodeKind kind; // node type
    Node *nodes[512]; // Nodes
    int val;       // Value
    int offset;    // Variable offset
    char str[64];  // Name
};

typedef struct LVar LVar;

// Type of Local variable.
struct LVar {
    LVar *next; // Next variable or NULL
    char *name; // Name
    int len;    // Length of name
    int offset; //Offset from RBP
};

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
