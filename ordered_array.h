#ifndef __ORDERED_ARRAY_H
#define __ORDERED_ARRAY_H

// GCC make make a regular character unsigned.
typedef signed char (*less_than_predicate_t)(void *, void *);
struct ordered_array {
	void **array;
	unsigned int size;
	unsigned int max_size;
	less_than_predicate_t less_than;
};

signed char standard_less_than_predicate(void *, void *);
struct ordered_array ordered_array_create(
	unsigned int,
	less_than_predicate_t
);

struct ordered_array ordered_array_place(
	void *,
	unsigned int,
	less_than_predicate_t
);

void ordered_array_remove(
	unsigned int,
	struct ordered_array *
);

void ordered_array_destroy(struct ordered_array *);
void ordered_array_insert(void *, struct ordered_array *);
#endif
