#include "dynamic-array.h"

static struct _dynamic_array_descriptor_t* __dynamic_arrays_descriptors;
static unsigned int __dynamic_arrays_current_count = 0;
static unsigned int __dynamic_arrays_allocated_count = 0;

dynamic_array_t _dynamic_create(
	unsigned int element_size
) {
	if (!__dynamic_arrays_descriptors) {
		__dynamic_arrays_descriptors = (struct _dynamic_array_descriptor_t*)
			calloc(DYNAMIC_ARRAY_EXPAND_LENGTH, sizeof(struct _dynamic_array_descriptor_t));
		if (!__dynamic_arrays_descriptors) return false;
	}
	else if (__dynamic_arrays_current_count == __dynamic_arrays_allocated_count) {
		struct _dynamic_array_descriptor_t* tmp = (struct _dynamic_array_descriptor_t*)realloc(__dynamic_arrays_descriptors,
			(__dynamic_arrays_allocated_count + DYNAMIC_ARRAY_EXPAND_LENGTH) * sizeof(struct _dynamic_array_descriptor_t));
		if (!tmp) return false;
		__dynamic_arrays_descriptors = tmp;
		__dynamic_arrays_allocated_count += DYNAMIC_ARRAY_EXPAND_LENGTH;
	}

	__dynamic_arrays_descriptors[__dynamic_arrays_current_count].dynamic_array = calloc(DYNAMIC_ARRAY_EXPAND_LENGTH, element_size);
	__dynamic_arrays_descriptors[__dynamic_arrays_current_count].element_size = element_size;
	__dynamic_arrays_descriptors[__dynamic_arrays_current_count].current_length = 0;
	__dynamic_arrays_descriptors[__dynamic_arrays_current_count].allocated_length = DYNAMIC_ARRAY_EXPAND_LENGTH;

	__dynamic_arrays_current_count += 1;

	return __dynamic_arrays_descriptors[__dynamic_arrays_current_count - 1].dynamic_array;
}

bool_t _dynamic_push(
	dynamic_array_t* dynamic_array_pointer,
	...
) {
	unsigned int index = 0;
	for (; index < __dynamic_arrays_current_count; index++) if (__dynamic_arrays_descriptors[index].dynamic_array == *dynamic_array_pointer) break;
	if (index == __dynamic_arrays_current_count) return false;

	if (__dynamic_arrays_descriptors[index].current_length >= __dynamic_arrays_descriptors[index].allocated_length) {
		dynamic_array_t tmp = realloc(
			__dynamic_arrays_descriptors[index].dynamic_array,
			(__dynamic_arrays_descriptors[index].allocated_length + DYNAMIC_ARRAY_EXPAND_LENGTH) * __dynamic_arrays_descriptors[index].element_size
		);

		if (!tmp) return false;

		__dynamic_arrays_descriptors[index].dynamic_array = tmp;
		__dynamic_arrays_descriptors[index].allocated_length += DYNAMIC_ARRAY_EXPAND_LENGTH;
		*dynamic_array_pointer = __dynamic_arrays_descriptors[index].dynamic_array;
	}

	unsigned char* next_elem_data = (unsigned char*)((unsigned int)__dynamic_arrays_descriptors[index].dynamic_array +
		__dynamic_arrays_descriptors[index].element_size * __dynamic_arrays_descriptors[index].current_length),
	* data = (unsigned char*)((unsigned int)&*dynamic_array_pointer + sizeof(dynamic_array_t));

	for (unsigned int j = 0; j < __dynamic_arrays_descriptors[index].element_size; j++) next_elem_data[j] = data[j];
	__dynamic_arrays_descriptors[index].current_length += 1;

	return true;
}

unsigned int dynamic_length(
	dynamic_array_t dynamic_array
) {
	unsigned int length = 0;

	for (unsigned int index = 0; index < __dynamic_arrays_current_count; index++) {
		if (__dynamic_arrays_descriptors[index].dynamic_array == dynamic_array) {
			length = __dynamic_arrays_descriptors[index].current_length;
			break;
		}
	}
	
	return length;
}

bool_t dynamic_free(
	dynamic_array_t dynamic_array
) {
	unsigned int index = 0;
	for (; index < __dynamic_arrays_current_count; index++) if (__dynamic_arrays_descriptors[index].dynamic_array == dynamic_array) break;
	if (index == __dynamic_arrays_current_count) return false;

	free(__dynamic_arrays_descriptors[index].dynamic_array);
	for (; index < __dynamic_arrays_current_count - 1; index++) __dynamic_arrays_descriptors[index] = __dynamic_arrays_descriptors[index + 1];
	__dynamic_arrays_current_count -= 1;

	if (!(__dynamic_arrays_current_count % DYNAMIC_ARRAY_EXPAND_LENGTH)) {
		void* tmp = realloc(__dynamic_arrays_descriptors,
			sizeof(dynamic_array_t) * (__dynamic_arrays_allocated_count - DYNAMIC_ARRAY_EXPAND_LENGTH));
		if (tmp) {
			__dynamic_arrays_descriptors = tmp;
			__dynamic_arrays_allocated_count -= DYNAMIC_ARRAY_EXPAND_LENGTH;
		}
	}

	return true;
}