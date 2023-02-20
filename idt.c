#include <idt.h>

void idt_set_gate(
	unsigned char num,
	unsigned long base,
	unsigned short sel,
	unsigned char flags
) {
	idt[num].base_low = (base & 0xffff);
	idt[num].base_high = (base >> 16) & 0xffff;
	idt[num].sel = sel;
	idt[num].always_zero = 0;
	idt[num].flags = flags;
}

void idt_install() {
	idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
	idtp.base = &idt;
	// memset(&idt, 0, sizeof(struct idt_entry) * 256);
	char *temp = (char *) &idt;
	for(int i = (sizeof(struct idt_entry) * 256); i != 0; i--) {
		*temp++ = 0;
	}

	idt_load();
}
