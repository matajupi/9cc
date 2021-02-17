.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 208
    call foo
    pop rax
    mov rsp, rbp
    pop rbp
    ret
