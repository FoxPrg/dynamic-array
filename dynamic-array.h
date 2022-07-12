#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

/*=================================
Simple Dynamic Arrays library for C
~~~Written by github.com/FoxPrg~~~~
=================================*/

#include <malloc.h>

#define DYNAMIC_ARRAY_EXPAND_LENGTH		10
#define DYNAMIC_ARRAY_BAD_INDEX_LENGTH	((unsigned int)-1)

#ifndef bool
typedef unsigned char bool_t;
#else
typedef bool bool_t;
#endif
#ifndef true
#define true	1
#endif
#ifndef false
#define false	0
#endif

typedef void* static_array_t, *dynamic_array_t;
struct _dynamic_array_descriptor_t {
	dynamic_array_t	dynamic_array;
	unsigned int	element_size;
	unsigned int	current_length;
	unsigned int	allocated_length;
};

dynamic_array_t _dynamic_create(
	unsigned int element_size
);
#define dynamic_create(...)	(__VA_ARGS__*)_dynamic_create(sizeof(__VA_ARGS__))

bool_t _dynamic_push(
	dynamic_array_t* dynamic_array_pointer,
	...
);
#define dynamic_push(...)	_dynamic_push(&__VA_ARGS__)

unsigned int dynamic_length(
	dynamic_array_t dynamic_array
);

bool_t dynamic_free(
	dynamic_array_t dynamic_array
);

#endif