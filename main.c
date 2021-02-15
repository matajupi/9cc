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
    tokenize(user_input);
    program();

    /// アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // Prologue
    // Reserve space for 26 variables
    printf("    push rbp\n");
    // ベースポインタにスタックポインタの値をムーブする
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n");
    
    // generate code from abstract syntax tree
    for (int i = 0; code[i]; i++) {
        gen(code[i]);
        // 評価結果のスタックをポップ
        printf("    pop rax\n");
    }

    // Epilogue
    // スタックポインタにベースポインタの値をムーブする
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
    return 0;
}