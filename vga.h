#ifndef __VGA_H
#define __VGA_H
void print_char(char);
void print_clear();
void print(char *);
#define println(s) \
	print( s ); \
	print("\n")

#endif
