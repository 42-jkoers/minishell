#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "malloc_preload.h"

// Simply a shared library to bind to, so that we override the calls in a preload

t_malloc_pointers get_pointers()
{
	return (t_malloc_pointers) {
		.ref_num_mallocs = NULL,
		.ref_num_frees = NULL,
		.ref_malloc_null_at = NULL,
		.ref_malloc_random = NULL,
		.ref_free_random = NULL
	};
}

void get_real_functions(void* malloc_ptr, void* free_ptr)
{
	*(void**)malloc_ptr = malloc;
	*(void**)free_ptr = free;
}
