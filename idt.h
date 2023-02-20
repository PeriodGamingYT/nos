#ifndef __IDT_H
#define __IDT_H
struct idt_entry {
	unsigned short base_low;
	unsigned short sel;
	unsigned char always_zero;
	unsigned char flags;
	unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;
void idt_set_gate(unsigned char, unsigned long, unsigned short, unsigned char);
void idt_install();
extern void idt_load();
#endif
