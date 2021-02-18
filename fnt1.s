.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 208
    mov rax, rsp
    mov rbx, 16
    cqo
    idiv rbx
    cmp rdx, 0
    je .Lend0
    mov rax, 8
    sub rsp, rax
.Lend0:
    push 2
    push 2
    push 2
    pop rdx
    pop rsi
    pop rdi
    call foo
    pop rax
    mov rsp, rbp
    pop rbp
    ret
