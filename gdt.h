#ifndef __GDT_H
#define __GDT_H
struct segment_descriptor {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_high;
	unsigned char type;
	unsigned char flags_limit_high;
	unsigned char base_vhigh;
} __attribute__((packed));

struct gdt {
	struct segment_descriptor null;
	struct segment_descriptor unused;
	struct segment_descriptor code;
	struct segment_descriptor data;
};

struct segment_descriptor segment_descriptor_make(
	unsigned int,
	unsigned int,
	unsigned char
);

unsigned int segment_descriptor_base(
	struct segment_descriptor
);

unsigned int segment_descriptor_limit(
	struct segment_descriptor
);

struct gdt gdt_make();
#endif
