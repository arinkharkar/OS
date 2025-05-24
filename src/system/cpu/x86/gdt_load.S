// GDT Loader implemented in GCC asm

// void __load_gdt(GdtDescriptor* gdt)
.global __load_gdt
__load_gdt:
    lgdt (%rdi)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

   # ; Far jump to reload CS with code segment selector 0x08
    pushq $0x08
    lea 1f(%rip), %rax
    pushq %rax
    lretq
1:
    ret
