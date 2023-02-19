.set IRQ_BASE, 0x20
.section .text
.extern int_handle

.macro HandleException num
global int_handle_exception\num\()Ev
int_handle_exception\num\()Ev:
    movb $\num, (int_num)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global handle_int_req\num\()Ev
handle_int_req\num\()Ev:
    movb $\num + IRQ_BASE, (int_num)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0c

int_bottom:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	pushl %esp
	push (int_handle)
	call int_handle
	# addl $5, %esp
	movl %eax, %esp
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
.global int_ignore
int_ignore:
    iret

.data
	int_num: .byte 0
