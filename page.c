#include "page.h"
#include "isr.h"
#include "common.h"
#include "vga.h"

unsigned int *frames;
unsigned int frames_num = 0;
extern unsigned int placement_address;
#define INDEX_FROM_BIT(x) ((x) / (8 * 4))
#define OFFSET_FROM_BIT(x) ((x) % (8 * 4))
static void set_frame(unsigned int frame_address) {
	unsigned int frame = frame_address / 0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int offset = OFFSET_FROM_BIT(frame);
	frames[index] |= (1 << offset);
}

static void clear_frame(unsigned int frame_address) {
	unsigned int frame = frame_address / 0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int offset = OFFSET_FROM_BIT(frame);
	frames[index] &= ~(1 << offset);
}

static unsigned int test_frame(unsigned int frame_address) {
	unsigned int frame = frame_address / 0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int offset = OFFSET_FROM_BIT(frame);
	return (frames[index] & (1 << offset));
}

static unsigned int first_frame() {
	for(
		unsigned int i = 0; 
		i < INDEX_FROM_BIT(frames_num); 
		i++
	) {
		if(frames[i] != 0xffffffff) {
			for(unsigned int j = 0; j < 32; j++) {
				unsigned int to_test = 1 << j;
				if(!(frames[i] & to_test)) {
					return i * 4 * 8 + j;
				}
			}
		}
	}
}

void allocate_frame(
	struct page *arg_page, 
	int is_kernel, 
	int is_writable
) {
	if(arg_page->frame != 0) {
		return;
	}

	unsigned int index = first_frame();
	if(index == (unsigned int) -1) {
		panic("no free frames!");
	}

	set_frame(index * 0x1000);
	arg_page->present = 1;
	arg_page->read_write = is_writable;
	arg_page->user = !is_kernel;
	arg_page->frame = index;
}

void free_frame(struct page *arg_page) {
	unsigned int frame = arg_page->frame;
	if(!frame) {
		return;
	}

	clear_frame(frame);
	arg_page->frame = 0;
}

struct page_directory *kernel_directory = 0;
struct page_directory *current_directory = 0;
void switch_page_directory(struct page_directory *new) {
	current_directory = new;
	asm volatile("mov %0, %%cr3" : : "r" (&new->table_physical));
	unsigned int cr0;
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x800000000;
	asm volatile("mov %0, %%cr0" : : "r" (cr0));
}

struct page *get_page(
	unsigned int address,
	int make,
	struct page_directory *directory
) {
	address /= 0x1000;
	unsigned int table_index = address / 1024;
	if(directory->tables[table_index]) {
		return (
			&directory->
			tables[table_index]->
			pages[address % 1024]
		);
	}

	if(make) {
		unsigned int temp;
		directory->tables[table_index] = (struct page_table *) 
			allocate_aligned_physical(sizeof(struct page_table), &temp);

		memory_set(directory->tables[table_index], 0, 0x1000);

		// 0x7 -> PRESENT, READ_WRITE, USER,
		directory->table_physical[table_index] = temp | 0x7;
		return &directory->tables[table_index]->pages[address % 1024];
	}

	return 0;
}

void page_fault(struct regs *arg_regs) {
	unsigned int faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	int present = !(arg_regs->error_code & 1);
	int read_write = arg_regs->error_code & 2;
	int user = arg_regs->error_code & 4;
	int reserved = arg_regs->error_code & 8;
	int id = arg_regs->error_code & 16;
	print("page fault ");
	if(present) {
		print("present ");
	}

	if(read_write) {
		print("read only ");
	}

	if(user) {
		print("user ");
	}

	if(reserved) {
		print("reserved ");
	}

	if(id) {
		print("instruction fetch ");
	}

	print("at ");
	print_num(faulting_address);
	println("");
	panic("");
}

void page_install() {
	unsigned int mem_end_page = 0x1000000;
	frames_num = mem_end_page / 0x1000;
	frames = (unsigned int *) allocate(INDEX_FROM_BIT(frames_num));
	memory_set(frames, 0, INDEX_FROM_BIT(frames_num));
	kernel_directory = (struct page_directory *) allocate_aligned(
		sizeof(struct page_directory)
	);

	memory_set(kernel_directory, 0, sizeof(struct page_directory));
	current_directory = kernel_directory;
	unsigned int i = 0;
	while(i < placement_address) {
		allocate_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	register_int_handler(14, page_fault);
	switch_page_directory(kernel_directory);
}
