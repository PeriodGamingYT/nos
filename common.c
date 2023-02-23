#include "common.h"
#include "vga.h"

void out_8(unsigned short port, unsigned char value) {
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

unsigned char in_8(unsigned short port) {
	unsigned char result;
	asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

unsigned short in_16(unsigned short port) {
	unsigned short result;
	asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
	return result;
}

void memory_set(void *dest, int value, int size) {
	char *temp = dest;
	while(size--) *temp++ = value;
}

void panic_no_macro(const char *message, const char *file, u32int line) {
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    print("panic (");
    print(message);
    print(") at ");
    print(file);
    print(":");
    print_dec(line);
    print("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, u32int line, const char *desc) {
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    print("assertion failed (");
    print(desc);
    print(") at ");
    print(file);
    print(":");
    print_dec(line);
    print("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
