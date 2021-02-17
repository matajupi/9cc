.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 208
    push 1
    push 2
    pop rsi
    pop rdi
    call foo
    pop rax
    mov rsp, rbp
    pop rbp
    ret
