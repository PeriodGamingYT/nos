#ifndef __COMMON_H
#define __COMMON_H
// #define COMMON_DEBUG
// #define COMMON_DEBUG_TIMER
#define COMMON_DEBUG_KEY
#ifdef COMMON_DEBUG
	#define COMMON_DEBUG_TIMER
	#define COMMON_DEBUG_KEY
#endif

void out_8(unsigned short port, unsigned char value);
unsigned char in_8(unsigned short port);
unsigned short in_16(unsigned short port);
void memory_set(void *, int, int);
#endif
