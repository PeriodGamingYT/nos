#include <gdt.h>

struct gdt_entry gdt[3];
struct gdt_ptr gp;
void gdt_set_gate(
	int num,
	unsigned long base,
	unsigned long limit,
	unsigned char access,
	unsigned char gran
) {
	gdt[num].base_low = (base & 0xffff);
	gdt[num].base_middle = (base >> 16) & 0xff;
	gdt[num].base_high = (base >> 24) & 0xff;
	gdt[num].limit_low = (limit & 0xffff);
	gdt[num].gran = ((limit >> 16) & 0x0f);
	gdt[num].gran |= (gran & 0xf0);
	gdt[num].access = access;
}

void gdt_install() {
	gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gp.base = &gdt;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);
	gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);
	gdt_flush();
}
