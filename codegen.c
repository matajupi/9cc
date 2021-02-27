#include "9cc.h"

int unique_number = 0;

// Get the address of the variable and push it onto the stack.
static void gen_lval(Node *node) {
    if (node->kind == ND_DEREF) {
        gen_lval(node->nodes[0]);
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    }
    if (node->kind != ND_LVAR)
        error("代入の左辺値が変数ではありません");

    // Compute the address from the base pointer
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n", node->offset);
    if (node->is_definition) {
            printf("    sub rsp, 8\n");
    }
    printf("    push rax\n");
}

// Generater assembly from node.
void gen(Node *node) {
    int pnum;
    int count;
    switch (node->kind) {
    case ND_NUM:
        printf("    push %d\n", node->val);
        return;
    case ND_ADDR:
        gen_lval(node->nodes[0]);
        return;
    case ND_DEREF:
        gen(node->nodes[0]);
        // kokoyorisitagaarukotoga issue
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    case ND_LVAR:
        gen_lval(node);
        // Get the value from the address and push it onto the stack
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    case ND_FNDEF:
        printf("%s:\n", node->str);
        // Prologue
        printf("    push rbp\n");
        printf("    mov rbp, rsp\n");

        // Get parameters and assign
        int i = 0;
        for (; node->nodes[i]; i++) {
            gen_lval(node->nodes[i]);
            printf("    pop rax\n");
            if (i == 5) printf("    mov [rax], r9\n");
            if (i == 4) printf("    mov [rax], r8\n");
            if (i == 3) printf("    mov [rax], rcx\n");
            if (i == 2) printf("    mov [rax], rdx\n");
            if (i == 1) printf("    mov [rax], rsi\n");
            if (i == 0) printf("    mov [rax], rdi\n");
        }

        gen(node->nodes[++i]);

        // Epilogue
        // 評価結果のスタックをポップ
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        return;
    case ND_FNCALL:
        pnum = unique_number++;
        printf("    mov rax, rsp\n");
        printf("    mov rbx, 16\n");
        printf("    cqo\n");
        printf("    idiv rbx\n");
        printf("    mov rax, rsp\n");
        printf("    cmp rdx, 0\n");
        printf("    je .Lsub%d\n", pnum);
        printf("    jmp .Lend%d\n", pnum);
        printf(".Lsub%d:\n", pnum);
        printf("    sub rsp, 8\n");
        printf(".Lend%d:\n", pnum);
        printf("    push rax\n");

        // Set real parameters
        for (int i = 0; node->nodes[i]; i++) {
            gen(node->nodes[i]);
            if (i == 5) printf("    pop r9\n");
            if (i == 4) printf("    pop r8\n");
            if (i == 3) printf("    pop rcx\n");
            if (i == 2) printf("    pop rdx\n");
            if (i == 1) printf("    pop rsi\n");
            if (i == 0) printf("    pop rdi\n");
        }

        printf("    call %s\n", node->str);
        printf("    pop rsp\n");
        printf("    push rax\n");
        return;
    case ND_ASSIGN:
        // Push the address of the variable on the left side onto the stack
        gen_lval(node->nodes[0]);

        // Push the value of right side onto the stack.
        gen(node->nodes[1]);
        
        // Pop the address and value and assign
        printf("    pop rdi\n");
        printf("    pop rax\n");
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
    case ND_RETURN:
        gen(node->nodes[0]);

        // Epilogue
        printf("    pop rax\n");
        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret \n");
        return;
    case ND_IF:
        pnum = unique_number++;
        gen(node->nodes[0]);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lelse%d\n", pnum);
        gen(node->nodes[1]);
        printf("    jmp .Lend%d\n", pnum);
        printf(".Lelse%d:\n", pnum);
        if (node->nodes[2])
            gen(node->nodes[2]);
        printf(".Lend%d:\n", pnum);
        return;
    case ND_ELSE:
        gen(node->nodes[0]);
        return;
    case ND_WHILE:
        pnum = unique_number++;
        printf(".Lbegin%d:", pnum);
        gen(node->nodes[0]);
        printf("    pop rax\n");
        printf("    cmp rax, 0\n");
        printf("    je .Lend%d\n", pnum);
        gen(node->nodes[1]);
        printf("    jmp .Lbegin%d\n", pnum);
        printf(".Lend%d:\n", pnum);
        return;
    case ND_FOR:
        pnum = unique_number++;
        if (node->nodes[0])
            gen(node->nodes[0]);
        printf(".Lbegin%d:\n", pnum);
        if (node->nodes[1]) {
            gen(node->nodes[1]);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je .Lend%d\n", pnum);
        }
        gen(node->nodes[3]);
        if (node->nodes[2])
            gen(node->nodes[2]);
        printf("    jmp .Lbegin%d\n", pnum);
        printf(".Lend%d:\n", pnum);
        return;
    case ND_BLOCK:
        count = 0;
        for (int i = 0; node->nodes[i]; i++)
            gen(node->nodes[i]);
        return;
    }

    gen(node->nodes[0]);
    gen(node->nodes[1]);

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