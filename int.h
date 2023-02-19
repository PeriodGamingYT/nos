#ifndef __INT_H
#define __INT_H
struct gate_descriptor {
	unsigned short handler_address_low;
	unsigned short gdt_code_segment;
	unsigned char reserved;
	unsigned char access;
	unsigned short handler_address_high;
} __attribute__((packed));

struct int_descriptor_table_ptr {
	unsigned short size;
	unsigned int base;
} __attribute__((packed));

struct int_manager;
struct int_handler {
	unsigned char int_num;
	struct int_manager *manager;
	unsigned int (*handler)(unsigned int);
};

struct int_manager {
	struct int_manager *active_int;
	struct int_handler *handlers[256];
	struct gate_descriptor int_table[256];
	unsigned short pic_master_command;
	unsigned short pic_master_data;
	unsigned short pic_slave_command;
	unsigned short pic_slave_data;
};

void set_int_descriptor_table_entry(
	struct int_manager *,
	unsigned char,
	unsigned short,
	void (*)(),
	unsigned char,
	unsigned char
);

struct gdt;
void int_activate(struct int_manager *, struct gdt *);
void int_deactivate(struct int_manager *);
unsigned int int_handle(
	struct int_manager *, 
	unsigned char, 
	unsigned int
);

unsigned int do_int_handle(
	struct int_manager *, 
	unsigned char, 
	unsigned int
);
#endif
