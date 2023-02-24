#ifndef __COMMON_H
#define __COMMON_H
// #define COMMON_DEBUG
// #define COMMON_DEBUG_TIMER
// #define COMMON_DEBUG_KEY
// #define COMMON_DEBUG_VM
#ifdef COMMON_DEBUG
	#define COMMON_DEBUG_TIMER
	#define COMMON_DEBUG_KEY
	#define COMMON_DEBUG_VM
#endif

typedef unsigned char u8int;
typedef signed char s8int;
typedef unsigned short u16int;
typedef signed short s16int;
typedef unsigned int u32int;
typedef signed int s32int;
#define panic(msg) panic_no_macro(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))
void panic_no_macro(const char *message, const char *file, u32int line);
void panic_assert(const char *file, u32int line, const char *desc);
void out_8(unsigned short port, unsigned char value);
unsigned char in_8(unsigned short port);
unsigned short in_16(unsigned short port);
void memory_set(void *, int, int);
#define memset memory_set
#endif
