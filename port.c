#include <port.h>

void port_8_write(unsigned short port, unsigned char data) {
	__asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

unsigned char port_8_read(unsigned short port) {
	unsigned char result;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}
