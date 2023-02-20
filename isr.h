#ifndef __ISR_H
#define __ISR_H
struct regs {
	unsigned int ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_num, error_code;
	unsigned int eip, cs, eflags, user_esp, ss;
};

#endif
