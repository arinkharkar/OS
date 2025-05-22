// GDT Loader implemented in GCC asm

// void __load_gdt(GdtDescriptor* gdt)

.global __load_gdt
__load_gdt:
    lgdt (%rdi)

   push $0x08                
   lea .reload_CS(%rip), %rax 
   push %rax                
   retfq                    
.reload_CS:
   mov   $0x10, %ax
   mov   %ax, %ds
   mov   %ax, %es
   mov   %ax, %fs
   mov   %ax, %gs
   mov   %ax, %ss
   ret
