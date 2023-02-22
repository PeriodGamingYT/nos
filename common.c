#include "common.h"

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

extern unsigned int end;
unsigned int placement_address = (unsigned int) &end;
static unsigned int internal_allocate(
	unsigned int size,
	int align,
	unsigned int *physical
) {
	if(align == 1 && (placement_address & 0x00000fff)) {
		placement_address &= 0x00000fff;
		placement_address += 0x1000;
	}

	if(physical) {
		*physical = placement_address;
	}

	unsigned int temp = placement_address;
	placement_address += size;
	return temp;
}

unsigned int allocate_aligned(unsigned int size) {
	return internal_allocate(size, 1, 0);
}

unsigned int allocate_physical(unsigned int size, unsigned int *physical) {
	return internal_allocate(size, 0, physical);
}

unsigned int allocate_aligned_physical(unsigned int size, unsigned int *physical) {
	return internal_allocate(size, 1, physical);
}

unsigned int allocate(unsigned int size) {
	return internal_allocate(size, 0, 0);
}
