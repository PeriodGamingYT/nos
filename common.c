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
