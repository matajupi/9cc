.intel_syntax noprefix
.globl main
main:
    push rbp
    mov rbp, rsp
    mov rax, rbp
    sub rax, 8
    sub rsp, 8
    push rax
    mov rax, rsp
    mov rbx, 16
    cqo
    idiv rbx
    cmp rdx, 0
    je .Lsub0
    jmp .Lend0
.Lsub0:
    sub rsp, 8
.Lend0:
    imul rax, 16
    push rax
    push 3
    pop rdi
    call fib
    pop rdi
    mov rsp, rdi
    push rax
    pop rdi
    pop rax
    mov [rax], rdi
    push rdi
    mov rax, rsp
    mov rbx, 16
    cqo
    idiv rbx
    cmp rdx, 0
    je .Lsub1
    jmp .Lend1
.Lsub1:
    sub rsp, 8
.Lend1:
    imul rax, 16
    push rax
    mov rax, rbp
    sub rax, 8
    push rax
    pop rax
    mov rax, [rax]
    push rax
    pop rdi
    call one
    pop rdi
    mov rsp, rdi
    push rax
    push rax
    pop rax
    mov rsp, rbp
    pop rbp
    ret
fib:
    push rbp
    mov rbp, rsp
    # ;assign n variable
    mov rax, rbp
    sub rax, 8
    sub rsp, 8
    push rax
    pop rax
    mov [rax], rdi
    # ;if statement
    # ;get n variable
    mov rax, rbp
    sub rax, 8
    push rax
    pop rax
    mov rax, [rax]
    push rax
    push 2
    # compare
    pop rdi
    pop rax
    cmp rax, rdi
    setl al
    movzb rax, al
    push rax
    pop rax
    cmp rax, 0
    je .Lelse2
    # return 1
    push 1
    pop rax
    mov rsp, rbp
    pop rbp
    ret 
    jmp .Lend2
.Lelse2:
.Lend2:
    # function call
    mov rax, rsp
    mov rbx, 16
    cqo
    idiv rbx
    cmp rdx, 0
    je .Lsub3
    jmp .Lend3
.Lsub3:
    sub rsp, 8
.Lend3:
    imul rax, 16
    push rax
    # get n
    mov rax, rbp
    sub rax, 8
    push rax
    pop rax
    mov rax, [rax]
    push rax
    push 1
    # compute n - 1
    pop rdi
    pop rax
    sub rax, rdi
    push rax
    # call
    pop rdi
    call fib
    pop rdi
    mov rsp, rdi
    push rax
    # function call
    mov rax, rsp
    mov rbx, 16
    cqo
    idiv rbx
    cmp rdx, 0
    je .Lsub4
    jmp .Lend4
.Lsub4:
    sub rsp, 8
.Lend4:
    imul rax, 16
    push rax
    # get n
    mov rax, rbp
    sub rax, 8
    push rax
    pop rax
    mov rax, [rax]
    push rax
    push 2
    # compute n - 2
    pop rdi
    pop rax
    sub rax, rdi
    push rax
    # call
    pop rdi
    call fib
    pop rdi
    mov rsp, rdi
    push rax
    # rax + rdi issue
    pop rdi
    pop rax
    add rax, rdi
    push rax
    pop rax
    mov rsp, rbp
    pop rbp
    ret 
    push rax
    pop rax
    mov rsp, rbp
    pop rbp
    ret
