#ifndef __VGA_H
#define __VGA_H
void terminal_clear();
void terminal_set_color(unsigned char);
void terminal_put_entry(
	char,
	unsigned char,
	int,
	int
);

void terminal_print_char(unsigned char);
void terminal_print(const unsigned char *);
#define print(x) terminal_print( x )
#define println(x) terminal_print( x ); terminal_print("\n")
#endif
