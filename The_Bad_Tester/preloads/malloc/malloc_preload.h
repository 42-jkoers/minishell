#ifndef MALLOC_PRELOAD_H
# define MALLOC_PRELOAD_H

#include <stdbool.h>

typedef struct s_malloc_pointers {
	int* ref_num_mallocs;
	int* ref_num_frees;
	int* ref_malloc_null_at;
	bool* ref_malloc_random;
	bool* ref_free_random;
}	t_malloc_pointers;

t_malloc_pointers get_pointers();
void get_real_functions(void* malloc_ptr, void* free_ptr);

#endif