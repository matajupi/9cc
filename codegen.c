#include "9cc.h"

int unique_number = 0;

static void gen_lval(Node *node) {
    if (node->kind != ND_LVAR)
        error("代入の左辺値が変数ではありません");

    // ベースポインタより変数のアドレスを計算
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    printf("    push rax\n");
}

void gen(Node *node) {
    int pnum;
    switch (node->kind) {
    case ND_NUM:
        printf("    push %d\n", node->val);
        return;
    case ND_LVAR:
        // 変数のアドレスを取得
        gen_lval(node);
        printf("    pop rax\n");
        // アドレスに入っている値を取得して積む
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    case ND_ASSIGN:
        // 変数のアドレスを積む
        gen_lval(node->lhs);
        // 右辺の値を積む
        gen(node->rhs);
        
        // 値の取り出し
        printf("    pop rdi\n");
        printf("    pop rax\n");
        // 左辺の変数のアドレス空間に右辺の値を代入
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
    case ND_RETURN:
        gen(node->lhs);
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret \n");
        return;
    case ND_IF:
        pnum = unique_number++;
        gen(node->lhs);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lelse%d\n", pnum);
        gen(node->rhs);
        printf("    jmp .Lend%d\n", pnum);
        printf(".Lelse%d:\n", pnum);
        if (node->rel)
            gen(node->rel);
        printf(".Lend%d:\n", pnum);
        return;
    case ND_ELSE:
        gen(node->lhs);
        return;
    case ND_WHILE:
        pnum = unique_number++;
        printf(".Lbegin%d:", pnum);
        gen(node->lhs);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lend%d\n", pnum);
        gen(node->rhs);
        printf("    jmp .Lbegin%d\n", pnum);
        printf(".Lend%d:\n", pnum);
        return;
    case ND_FOR:
        pnum = unique_number++;
        gen(node->lhs);
        printf(".Lbegin%d:\n", pnum);
        gen(node->rel);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lend%d\n", pnum);
        gen(node->rhs);
        gen(node->upd);
        printf("    jmp .Lbegin%d\n", pnum);
        printf(".Lend%d:\n", pnum);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("    add rax, rdi\n");
        break;
    case ND_SUB:
        printf("    sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("    imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("    cqo\n");
        printf("    idiv rdi\n");
        break;
    case ND_EQ:
        printf("    cmp rax, rdi\n");
        printf("    sete al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_NE:
        printf("    cmp rax, rdi\n");
        printf("    setne al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_LT:
        printf("    cmp rax, rdi\n");
        printf("    setl al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_LE:
        printf("    cmp rax, rdi\n");
        printf("    setle al\n");
        printf("    movzb rax, al\n");
        break;
    }

    printf("    push rax\n");
}