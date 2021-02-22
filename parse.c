#include "9cc.h"

static bool consume(char *op);
static Token *consume_kind(TokenKind tk);
static void expect(char *op);
static void expect_kind(TokenKind tk);
static int expect_number();
static bool at_eof();

// When the next token is the expected symbol, it consumes one token and return true.
// Otherwise it returns false.
static bool consume(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

// When the next token kind is the expected kind, it consumes one token and return
// that token. Otherwise it returns NULL.
static Token *consume_kind(TokenKind tk) {
    if (token->kind != tk)
        return NULL;
    Token *retval = token;
    token = token->next;
    return retval;
}

// When the next token is the expected symbol, it consumes one token.
// Otherwise it reports an error.
static void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        error_at(token->str, "'%s'ではありません", op);
    token = token->next;
}

// When the next token kind it the expected kind, it consume one token.
// Otherwise it reports an error.
static void expect_kind(TokenKind tk) {
    if (token->kind != tk)
        error_at(token->str, "トークンの種類が不正です");
    token = token->next;
}

// When the next token is a number, it reads one token and returns that number.
// Otherwise it reports an error.
static int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

// Whether the current token is EOF.
static bool at_eof() {
    return token->kind == TK_EOF;
}


static Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
static Node *new_node_num(int val);

// Create a new node from the kind, left and right sides.
static Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->nodes[0] = lhs;
    node->nodes[1] = rhs;
    return node;
}

// Create a new numeric node from a given number.
static Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}


static LVar *find_lvar(Token *tok);

// Current routine's local variables
LVar *locals;

// Finds a local variable from the given token and the current routine
// and returns that variable if it exists. Other wise returns NULL.
static LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    }
    return NULL;
}


void program();
Node *func();
Node *block();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();
Node *ident();

// All nodes
Node *code[100];

// program = (func | // assign)*
void program() {
    int i = 0;
    LVar *head = calloc(1, sizeof(LVar));
    while (!at_eof()) {
        code[i++] = func();
        // TODO Later: Implement global ident
    }
    code[i] = NULL;
}

// func = ident "(" ident* ")" block
// ND_FNDEF nodes = {ident*(parameters), NULL, block}
Node *func() {
    // Initialize node
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FNDEF;

    // Set node name
    Token *tok = consume_kind(TK_IDENT);
    memcpy(node->str, tok->str, tok->len);
    (node->str)[tok->len] = '\0';

    // Initialize local variables
    LVar *head = calloc(1, sizeof(LVar));
    head->offset = 0;
    locals = head;

    // Set parameters
    expect("(");
    int i = 0;
    node->nodes[0] = NULL;
    for (; !consume(")"); i++) {
        Token *tok = consume_kind(TK_IDENT);
        node->nodes[i] = ident(tok);
        if (!consume(","))
            node->nodes[i + 1] = NULL;
    }
    
    // Set block
    expect("{");
    node->nodes[++i] = block();
    return node;
}

// block = "{" stmt* "}"
// ND_BLOCK nodes = {stmt*, NULL}
Node *block() {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_BLOCK;
    int i;
    for (i = 0; !consume("}"); i++) {
        node->nodes[i] = stmt();
    }
    node->nodes[i + 1] = NULL;
    return node;
}

// stmt = expr ";"
//      | "if" "(" expr ")" stmt ("else" stmt)?
//      | "while" "(" expr ")" stmt
//      | "for" "(" expr? ";" expr? ";" expr? ")" stmt
//      | "return" expr ";"
//      | block
Node *stmt() {
    Node *node;
    node = calloc(1, sizeof(Node));

    // ND_RETURN nodes = {expr}
    if (consume_kind(TK_RETURN)) {
        node->kind = ND_RETURN;
        node->nodes[0] = expr();
        expect(";");
        return node;

    // ND_IF nodes = {expr(condition), stmt(case if), ND_ELSE | NULL}
    } else if (consume_kind(TK_IF)) {
        node->kind = ND_IF;
        expect("(");
        node->nodes[0] = expr();
        expect(")");
        node->nodes[1] = stmt();
        node->nodes[2] = NULL;
        
        // ND_ELSE nodes = {stmt(case else)}
        if (consume_kind(TK_ELSE)) {
            Node *rel = calloc(1, sizeof(Node));
            rel->kind = ND_ELSE;
            rel->nodes[0] = stmt();
            node->nodes[2] = rel;
        }
        return node;

    // ND_WHILE nodes = {expr(condition), stmt}
    } else if (consume_kind(TK_WHILE)) {
        node->kind = ND_WHILE;
        expect("(");
        node->nodes[0] = expr();
        expect(")");
        node->nodes[1] = stmt();
        return node;

    // ND_FOR nodes = {expr | NULL, expr | NULL, expr | NULL, stmt}
    } else if (consume_kind(TK_FOR)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        expect("(");
        if (consume(";")) {
            node->nodes[0] = NULL;
        } else {
            node->nodes[0] = expr();
            expect(";");
        }
        if (consume(";")) {
            node->nodes[1] = NULL;
        } else {
            node->nodes[1] = expr();
            expect(";");
        }
        if (consume(")")) {
            node->nodes[2] = NULL;
        } else {
            node->nodes[2] = expr();
            expect(")");
        }
        node->nodes[3] = stmt();
        return node;

    } else if (consume("{")) {
        node = block();
        return node;

    } else {
        node = expr();
        expect(";");
        return node;

    }
}

// expr = assign
Node *expr() {
    return assign();
}

// assign = equality ("=" assign)?
Node *assign() {
    Node *node = equality();
    if (consume("="))
        node = new_node(ND_ASSIGN, node, assign());
    return node;
}

// equality = relational ("==" relational | "!=" relational)*
Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_node(ND_NE, node, relational());
        else
            return node;
    }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume("<="))
            node = new_node(ND_LE, node, add());
        else if (consume(">"))
            node = new_node(ND_LT, add(), node);
        else if (consume(">="))
            node = new_node(ND_LE, add(), node);
        else
            return node;
    }
}
// add = mul ("+" mul | "-" mul)*
Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

// unary = ("+" | "-")* primary
Node *unary() {
    if (consume("+"))
        return unary();
    if (consume("-"))
        // 0 - num
        return new_node(ND_SUB, new_node_num(0), unary());
    return primary();
}

// primary = num 
//          | ident ("(" expr* ")")?
//          | "(" expr ")"
Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }
    Token *tok = consume_kind(TK_IDENT);
    if (tok) { 
        return ident(tok);
    }
    Node *node = new_node_num(expect_number());
    return node;
}

// ident related processing
Node *ident(Token *tok) {
    Node *node = calloc(1, sizeof(Node));

    // ND_FNCALL nodes = {expr*(real parameters), NULL}
    if (consume("(")) {
        node->kind = ND_FNCALL;
        memcpy(node->str, tok->str, tok->len);
        (node->str)[tok->len] = '\0';
        node->nodes[0] = NULL;
        for (int i = 0; !consume(")"); i++) {
            node->nodes[i] = expr();
            if (!consume(","))
                node->nodes[i + 1] = NULL;
        }
        return node;
    }

    node->kind = ND_LVAR;
    LVar *lvar = find_lvar(tok);
    if (lvar) {
        node->offset = lvar->offset;
    } else {
        lvar = calloc(1, sizeof(LVar));
        lvar->next = locals;
        lvar->name = tok->str;
        lvar->offset = locals->offset + 8;
        node->offset = lvar->offset;
        locals = lvar;
    }
    return node;
}
