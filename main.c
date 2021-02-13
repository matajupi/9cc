#include "9cc.h"

char *user_input;
Token *token;

int main(int argc, char **argv) {
    if (argc != 2) {
        error("引数の個数が正しくありません");
        return 1;
    }

    // tokenize and parse
    user_input = argv[1];
    token = tokenize(user_input);
    Node *node = expr();

    /// アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    
    // generate code from abstract syntax tree
    gen(node);

    // load rax from top of stack
    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}