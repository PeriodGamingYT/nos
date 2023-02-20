#ifndef __COMMON_H
#define __COMMON_H
void out_8(unsigned short port, unsigned char value);
unsigned char in_8(unsigned short port);
unsigned short in_16(unsigned short port);
void memory_set(void *, int, int);
#endif
