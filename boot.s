MULTIBOOT_PAGE_ALIGN equ 1 << 0
MULTIBOOT_MEM_INFO equ 1 << 1
MULTIBOOT_VIDEO_INFO equ 1 << 2
MULTIBOOT_HEADER_MAGIC equ 0x1badb002
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEM_INFO
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[BITS 32]
[GLOBAL multiboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]
multiboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
	dd multiboot
	dd code
	dd bss
	dd end
	dd start

[GLOBAL start]
[EXTERN main]
start:
	push ebx
	cli
	call main
	jmp $
