.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp

    mov rax, rsp
    sub rax, 8
    mov rdi, 3
    mov [rax], rdi

    mov rax, rsp
    sub rax, 8
    // ?
    push rax
    pop rax
    // ?
    mov rax, [rax]

    mov rsp, rbp
    pop rbp
    ret
     