#include <int.h>
#include <gdt.h>
#include <port.h>

void set_int_descriptor_table_entry(
	struct int_manager *manager,
	unsigned char int_num,
	unsigned short gdt_code_segment,
	void (*handler)(),
	unsigned char descriptor_privilege,
	unsigned char descriptor_type
) {
	const unsigned char IDT_DESC_PRESENT = 0x80;
	manager->int_table[int_num].handler_address_low = 
		((unsigned int) handler) & 0xffff;

	manager->int_table[int_num].handler_address_high = 
		(((unsigned int) handler) >> 16) & 0xffff;

	manager->int_table[int_num].gdt_code_segment = gdt_code_segment;
	manager->int_table[int_num].access = 
		IDT_DESC_PRESENT |
		descriptor_type |
		(descriptor_privilege << 5);

	manager->int_table[int_num].reserved = 0;
}

void int_activate(
	struct int_manager *manager,
	struct gdt *arg_gdt
) {
	if(manager->active_int != 0) {
		int_deactivate(manager->active_int);
	}

	manager->active_int = manager;
	__asm__("sti");
}

void int_deactivate(
	struct int_manager *manager
) {
	if(manager->active_int == manager) {
		manager->active_int = 0;
		__asm__("cli");
	}
}

unsigned int int_handle(
	struct int_manager *manager,
	unsigned char int_num,
	unsigned int esp
) {
	if(manager->active_int != 0) {
		return do_int_handle(manager->active_int, int_num, esp);
	}

	return esp;
}

#include <vga.h>
unsigned int do_int_handle(
	struct int_manager *manager,
	unsigned char int_num,
	unsigned int esp
) {
	terminal_print("hit");
	if(manager->handlers[int_num] != 0) {
		esp = int_handle(manager, int_num, esp);
	}

	if(int_num != 0x20) {
		terminal_print("unhandled int");
	}

	if(0x20 <= int_num && int_num < 0x30) {
		port_8_slow_write(manager->pic_master_command, 0x20);
		if(0x28 <= int_num) {
			port_8_slow_write(manager->pic_slave_command, 0x20);
		}
	}
}

static void int_ignore() {}
static void handle_int_req_0x00() {}
static void handle_int_req_0x01() {}
static void handle_int_req_0x0c() {}
struct int_manager int_manager_make(struct gdt *arg_gdt) {
	struct int_manager result;
	result.pic_master_command = 0x20;
	result.pic_master_data = 0x21;
	result.pic_slave_command = 0xa0;
	result.pic_slave_data = 0xa1;
	unsigned short code = gdt_code(arg_gdt);
	const unsigned char IDT_INTERRUPT_GATE = 0xe;
	for(unsigned short i = 0; i < 256; i++) {
		set_int_descriptor_table_entry(
			&result,
			i,
			code,
			&int_ignore,
			0,
			IDT_INTERRUPT_GATE
		);

		result.handlers[i] = 0;
	}

	set_int_descriptor_table_entry(
		&result, 0x20, code, &handle_int_req_0x00, 0, IDT_INTERRUPT_GATE
	);

	set_int_descriptor_table_entry(
		&result, 0x21, code, &handle_int_req_0x01, 0, IDT_INTERRUPT_GATE
	);

	set_int_descriptor_table_entry(
		&result, 0xc0, code, &handle_int_req_0x0c, 0, IDT_INTERRUPT_GATE
	);

	port_8_slow_write(result.pic_master_command, 0x11);
	port_8_slow_write(result.pic_master_data, 0x20);
	port_8_slow_write(result.pic_slave_command, 0x11);
	port_8_slow_write(result.pic_slave_data, 0x28);
	port_8_slow_write(result.pic_master_data, 0x04);
	port_8_slow_write(result.pic_slave_data, 0x02);
	port_8_slow_write(result.pic_master_data, 0x01);
	port_8_slow_write(result.pic_slave_data, 0x01);
	port_8_slow_write(result.pic_master_data, 0x00);
	port_8_slow_write(result.pic_slave_data, 0x00);
	struct int_descriptor_table_ptr idt = {
		.size = 256 * sizeof(struct gate_descriptor) - 1,
		.base = (unsigned int) result.int_table
	};

	__asm__ volatile("lidt %0" : : "m" (idt));
	return result;
}
