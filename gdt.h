#ifndef __GDT_H
#define __GDT_H
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char gran;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void gdt_set_gate(int, unsigned long, unsigned long, unsigned char, unsigned char);
void gdt_install();
extern void gdt_flush();
#endif
