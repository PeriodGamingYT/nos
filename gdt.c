#include <gdt.h>

struct segment_descriptor segment_descriptor_make(
	unsigned int base,
	unsigned int limit,
	unsigned char flags
) {
	struct segment_descriptor result;
	unsigned char *target = (unsigned char *) &result;
	target[6] = 0x40;
	if(limit > 65536) {
		target[6] = 0xc0;
		int old_limit = limit;
		limit >>= 12;
		if((limit & 0xfff) != 0xfff) {
			limit = (old_limit >> 12) - 1;
		}
	}

	target[0] = limit & 0xff;
	target[1] = (limit >> 8) & 0xff;
	target[2] = base & 0xff;
	target[3] = (base >> 8) & 0xff;
	target[4] = (base >> 16) & 0xff;
	target[5] = flags;
	target[6] |= (limit >> 16) & 0xf;
	target[7] = (base >> 24) & 0xff;
	return result;
}

unsigned int segment_descriptor_base(
	struct segment_descriptor descriptor
) {
	unsigned char *target = (unsigned char *) &descriptor;
	unsigned int result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
	return result;
}

unsigned int segment_descriptor_limit(
	struct segment_descriptor descriptor
) {
	unsigned char *target = (unsigned char *) &descriptor;
	unsigned int result = target[6] & 0xf;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];
	if((target[6] & 0xc0) == 0xc0) {
		result = (result << 12) | 0xfff;
	}
	
	return result;
}

struct gdt gdt_make() {
	struct gdt result = {
		.null = segment_descriptor_make(0, 0, 0),
		.unused = segment_descriptor_make(0, 0, 0),
		.code = segment_descriptor_make(0, 64 * 1024 * 1024, 0x9a),
		.data = segment_descriptor_make(0, 64 * 1024 * 1024, 0x92)
	};

	unsigned int i[2];
	i[1] = (unsigned int) &result;
	i[0] = sizeof(struct gdt) << 16;
	__asm__ volatile("lidtl (%0)" : : "r" (&i));
	return result;
}

unsigned short gdt_data(struct gdt *arg_gdt) {
	return (unsigned char *) &arg_gdt->data - (unsigned char *) arg_gdt;
}

unsigned short gdt_code(struct gdt *arg_gdt) {
	return (unsigned char *) &arg_gdt->code - (unsigned char *) arg_gdt;
}
