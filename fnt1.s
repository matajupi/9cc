.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 208
    push 1
    push 1
    pop rsi
    pop rdi
    mov rbx, rdx
    mov rax, rsp
    mov rcx, 16
    cqo
    idiv rcx
    cmp rdx, 0
    je .Lend0
    mov rax, 8
    add rsp, rax
.Lend0:
    mov rdx, rbx
    call huga
    pop rax
    mov rsp, rbp
    pop rbp
    ret
