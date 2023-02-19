#ifndef __PORT_H
#define __PORT_H
// Functions that are commented out might be implemented later into development.
// void port_8_write(unsigned short, unsigned char);
// unsigned char port_8_read(unsigned short);
void port_8_slow_write(unsigned short, unsigned char);
unsigned char port_8_slow_read(unsigned short);
// void port_16_write(unsigned short, unsigned short);
// unsigned short port_16_read(unsigned short);
// void port_32_write(unsigned short, unsigned int);
// unsigned int port_32_read(unsigned short, unsigned int);
#endif
