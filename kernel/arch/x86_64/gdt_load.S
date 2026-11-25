.section .text

.global gdt_load
.type gdt_load, @function

gdt_load:
    lgdt (%rdi)

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss

    pushq $0x08
    leaq .reload_cs(%rip), %rax
    pushq %rax
    lretq

.reload_cs:
    ret

.size gdt_load, .-gdt_load

.section .note.GNU-stack,"",@progbits
