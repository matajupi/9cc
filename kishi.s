.globl main
main:
    sub $16, %rsp
    mov %rsp, %rax
    add $8, %rax
    push %rax
    pop %rdx
    mov $99, %rax
    mov %rax, (%rdx)

    mov $0, %rax
    mov $0, %rdx

    mov %rsp, %rax
    add $8, %rax
    push %rax
    pop %rdx
    mov $0, %rax
    mov (%rdx), %rax
    mov $0, %rdx
    ret
