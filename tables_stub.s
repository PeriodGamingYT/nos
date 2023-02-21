[GLOBAL gdt_flush]
gdt_flush:
	mov eax, [esp + 4]
	lgdt [eax]
	jmp 0x08:after_gdt_flush
after_gdt_flush:
	mov ax, 0x10
	mov dx, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

[GLOBAL idt_flush]
idt_flush:
	mov eax, [esp + 4]
	lidt [eax]
	ret
