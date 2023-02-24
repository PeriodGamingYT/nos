#include "vga.h"
#include "tables.h"
#include "key.h"
#include "timer.h"
#include "vm.h"
#include "paging.h"

#define KERNEL_CODE_MAX 72
void main(struct multiboot *multiboot_ptr) {
	init_tables();
	print_clear();
	asm volatile("sti");
	timer_install(50);
	keyboard_install();
	// initialise_paging();
	unsigned char kernel_code[] = {
		12, 1,
		6, 0, 'n',
		6, 1, 'o',
		6, 2, 's',
		6, 3, ' ',
		6, 4, 's',
		6, 5, 't',
		6, 6, 'a',
		6, 7, 'r',
		6, 8, 't',
		6, 9, 'e',
		6, 10, 'd',
		6, 11, '\n',
		13, 0,
		11
	};

	struct vm kernel_vm = vm_create(kernel_code, KERNEL_CODE_MAX);
	vm_add(&kernel_vm, 15);
	vm_install();
	for(;;);
}
