.set MAGIC, 0x1badb002
.set FLAGS, (1 << 0 | 1 << 1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

.section .text
.global gdt_flush
.extern gp
gdt_flush:
   lgdt gp
   mov %ax, 0x10
   mov %dx, %ax
   mov %es, %ax
   mov %fs, %ax
   mov %gs, %ax
   mov %ss, %ax
   jmp $0x08, $flush2

flush2:
	ret
   
  
.extern kernel_main
.global loader

loader:
	mov $kernel_stack, %esp
	push %eax
	push %ebx
	call kernel_main

_stop:
	cli
	hlt
	jmp _stop

.section .bss
.space 2*1024*1024 ; # 2 MiB.

kernel_stack:
