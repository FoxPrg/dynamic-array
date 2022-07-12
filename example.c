#include "dynamic-array.h"
#include <stdio.h>

int main() {
	int* integers = dynamic_create(int);
	for (unsigned int i = 0; i < 0xff; i++) printf("%u) Push status: %u\r\n", i, dynamic_push(integers, i));
	for (unsigned int i = 0; i < 0xff; i++) printf("%u) %u", i, integers[i]);
	printf("Free status: %u\r\n", dynamic_free(integers));
}