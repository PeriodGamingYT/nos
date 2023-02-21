#include "tables.h"
#include "common.h"
#include "isr.h"

extern void gdt_flush(unsigned int);
struct gdt_entry gdt_entries[5];
struct gdt_ptr gdt_entries_ptr;
static void gdt_set_gate(
	int num,
	unsigned int base,
	unsigned int limit,
	unsigned char access,
	unsigned char gran
) {
	gdt_entries[num].base_low = (base & 0xffff);
	gdt_entries[num].base_middle = (base >> 16) & 0xff;
	gdt_entries[num].base_high = (base >> 24) & 0xff;
	gdt_entries[num].limit_low = (limit & 0xffff);
	gdt_entries[num].gran = (limit >> 16) & 0x0f;
	gdt_entries[num].gran |= gran & 0xf0;
	gdt_entries[num].access = access;
}

static void init_gdt() {
	gdt_entries_ptr.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gdt_entries_ptr.base = (unsigned int) &gdt_entries;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);
	gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);
	gdt_set_gate(3, 0, 0xffffffff, 0xfa, 0xcf);
	gdt_set_gate(4, 0, 0xffffffff, 0xf2, 0xcf);
	gdt_flush((unsigned int) &gdt_entries_ptr);
}

struct idt_entry idt_entries[256];
struct idt_ptr idt_entries_ptr;
extern void idt_flush(unsigned int);
static void idt_set_gate(
	unsigned char num,
	unsigned int base,
	unsigned short kernel_segment,
	unsigned char flags
) {
	idt_entries[num].base_low = base & 0xffff;
	idt_entries[num].base_high = (base >> 16) & 0xffff;
	idt_entries[num].kernel_segment = kernel_segment;
	idt_entries[num].reserved = 0;
	idt_entries[num].flags = flags;
}

static void init_idt() {
	idt_entries_ptr.limit = (sizeof(struct idt_entry) * 256) - 1;
	idt_entries_ptr.base = (unsigned int) &idt_entries;
	memory_set(&idt_entries, 0, sizeof(struct idt_entry) * 256);
	out_8(0x20, 0x11);
	out_8(0xa0, 0x11);
	out_8(0x21, 0x20);
	out_8(0xa1, 0x28);
	out_8(0x21, 0x04);
	out_8(0xa1, 0x02);
	out_8(0x21, 0x01);
	out_8(0xa1, 0x01);
	out_8(0x21, 0x0);
	out_8(0xa1, 0x0);
	idt_set_gate(0, (unsigned int) isr0, 0x08, 0x8e);
	idt_set_gate(1, (unsigned int) isr1, 0x08, 0x8e);
	idt_set_gate(2, (unsigned int) isr2, 0x08, 0x8e);
	idt_set_gate(3, (unsigned int) isr3, 0x08, 0x8e);
	idt_set_gate(4, (unsigned int) isr4, 0x08, 0x8e);
	idt_set_gate(5, (unsigned int) isr5, 0x08, 0x8e);
	idt_set_gate(6, (unsigned int) isr6, 0x08, 0x8e);
	idt_set_gate(7, (unsigned int) isr7, 0x08, 0x8e);
	idt_set_gate(8, (unsigned int) isr8, 0x08, 0x8e);
	idt_set_gate(9, (unsigned int) isr9, 0x08, 0x8e);
	idt_set_gate(10, (unsigned int) isr10, 0x08, 0x8e);
	idt_set_gate(11, (unsigned int) isr11, 0x08, 0x8e);
	idt_set_gate(12, (unsigned int) isr12, 0x08, 0x8e);
	idt_set_gate(13, (unsigned int) isr13, 0x08, 0x8e);
	idt_set_gate(14, (unsigned int) isr14, 0x08, 0x8e);
	idt_set_gate(15, (unsigned int) isr15, 0x08, 0x8e);
	idt_set_gate(16, (unsigned int) isr16, 0x08, 0x8e);
	idt_set_gate(17, (unsigned int) isr17, 0x08, 0x8e);
	idt_set_gate(18, (unsigned int) isr18, 0x08, 0x8e);
	idt_set_gate(19, (unsigned int) isr19, 0x08, 0x8e);
	idt_set_gate(20, (unsigned int) isr20, 0x08, 0x8e);
	idt_set_gate(21, (unsigned int) isr21, 0x08, 0x8e);
	idt_set_gate(22, (unsigned int) isr22, 0x08, 0x8e);
	idt_set_gate(23, (unsigned int) isr23, 0x08, 0x8e);
	idt_set_gate(24, (unsigned int) isr24, 0x08, 0x8e);
	idt_set_gate(25, (unsigned int) isr25, 0x08, 0x8e);
	idt_set_gate(26, (unsigned int) isr26, 0x08, 0x8e);
	idt_set_gate(27, (unsigned int) isr27, 0x08, 0x8e);
	idt_set_gate(28, (unsigned int) isr28, 0x08, 0x8e);
	idt_set_gate(29, (unsigned int) isr29, 0x08, 0x8e);
	idt_set_gate(30, (unsigned int) isr30, 0x08, 0x8e);
	idt_set_gate(31, (unsigned int) isr31, 0x08, 0x8e);
	idt_set_gate(32, (unsigned int) irq0, 0x08, 0x8e);
	idt_set_gate(33, (unsigned int) irq1, 0x08, 0x8e);
	idt_set_gate(34, (unsigned int) irq2, 0x08, 0x8e);
	idt_set_gate(35, (unsigned int) irq3, 0x08, 0x8e);
	idt_set_gate(36, (unsigned int) irq4, 0x08, 0x8e);
	idt_set_gate(37, (unsigned int) irq5, 0x08, 0x8e);
	idt_set_gate(38, (unsigned int) irq6, 0x08, 0x8e);
	idt_set_gate(39, (unsigned int) irq7, 0x08, 0x8e);
	idt_set_gate(40, (unsigned int) irq8, 0x08, 0x8e);
	idt_set_gate(41, (unsigned int) irq9, 0x08, 0x8e);
	idt_set_gate(42, (unsigned int) irq10, 0x08, 0x8e);
	idt_set_gate(43, (unsigned int) irq11, 0x08, 0x8e);
	idt_set_gate(44, (unsigned int) irq12, 0x08, 0x8e);
	idt_set_gate(45, (unsigned int) irq13, 0x08, 0x8e);
	idt_set_gate(46, (unsigned int) irq14, 0x08, 0x8e);
	idt_set_gate(47, (unsigned int) irq15, 0x08, 0x8e);
	idt_set_gate(128, (unsigned int) isr128, 0x08, 0x8e);
	idt_flush((unsigned int) &idt_entries_ptr);
}

extern isr_t int_handlers[];
void init_tables() {
	init_gdt();
	init_idt();
	memory_set(&int_handlers, 0, sizeof(isr_t) * 256);
}
