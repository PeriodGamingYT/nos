#include <port.h>

// void port_8_write(unsigned short port, unsigned char data) {
	// __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
// }
// 
// unsigned char port_8_read(unsigned short port) {
	// unsigned char result;
	// __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
	// return result;
// }

void port_8_slow_write(unsigned short port, unsigned char data) {
	__asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port));
}

unsigned char port_8_slow_read(unsigned short port) {
	unsigned char result;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

// void port_16_write(unsigned short port, unsigned short data) {
	// __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port));
// }
// 
// unsigned short port_16_read(unsigned short port) {
	// unsigned short result;
	// __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
	// return result;
// }

// void port_32_write(unsigned short port, unsigned int data) {
	// __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port));
// }
// 
// unsigned int port_32_read(unsigned short port) {
	// unsigned int result;
	// __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
	// return result;
// }
