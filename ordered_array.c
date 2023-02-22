#include "ordered_array.h"
#include "common.h"

signed char standard_less_than_predicate(void *a, void *b) {
	return a < b ? 1 : 0;
}

struct ordered_array ordered_array_create(
	unsigned int max_size,
	less_than_predicate_t less_than
) {
	ordered_array result;
	result.array = (void *) allocate(max_size * sizeof(void *));
	memory_set(result.array, 0, max_size * sizeof(void *));
	result.size = 0;
	result.max_size = max_size;
	result.less_than = less_than;
	return result;
}

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
