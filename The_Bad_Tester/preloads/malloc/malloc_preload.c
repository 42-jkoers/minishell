#include "override.h"
#include "malloc_preload.h"
#include <stdbool.h>

#if __linux__
# include <malloc.h>
# define MALLOC_SIZE malloc_usable_size
#else
# include "malloc/malloc.h"
# define MALLOC_SIZE malloc_size
#endif

#pragma region override function defines

# define malloc_param_args size_t size
# define malloc_fake_func my_malloc
# define malloc_return_type void*
# define malloc_call_args size

# define free_param_args void* ptr
# define free_fake_func my_free
# define free_return_type void
# define free_call_args ptr

# define get_pointers_param_args void
# define get_pointers_fake_func my_get_pointers
# define get_pointers_return_type t_malloc_pointers
# define get_pointers_call_args

# define get_real_functions_param_args void* malloc_ptr, void* free_ptr
# define get_real_functions_fake_func my_get_real_functions
# define get_real_functions_return_type void
# define get_real_functions_call_args malloc_ptr, free_ptr

#pragma endregion

#pragma region boostrap functions

# define bootstrap_size 1024*32 // 32K ought to be enough (define because static const complained)
static char bootstrap_data[bootstrap_size];
static size_t bootstrap_loc = 0;
static bool is_bootstrapping = true;

void* bootstrap_malloc(size_t size)
{
	void* loc = &bootstrap_data[bootstrap_loc];
	bootstrap_loc += size;

	if (bootstrap_loc > bootstrap_size)
	{
		printf("Failed to complete bootstrap!\nRan out of bootstrap malloc space!\n");
		exit(1);
	}

	return loc;
}
void bootstrap_free(void* ptr) { }

PRE_OVERRIDE_WITH_DEFAULT(malloc, bootstrap_malloc)
PRE_OVERRIDE_WITH_DEFAULT(free, bootstrap_free)

#pragma endregion

#pragma region util functions

static void randomize_ptr(unsigned char* ptr, int length)
{
	if (ptr)
		for (int i = 0; i < length; i++)
			ptr[i] = (unsigned char)rand();
}

#pragma endregion

#pragma region override library functions
static int num_mallocs = 0;
static int num_frees = 0;
static int malloc_null_at = -1;
static bool malloc_random = true;
static bool free_random = true;

t_malloc_pointers my_get_pointers()
{
	if (is_bootstrapping)
	{
		is_bootstrapping = false;
		num_mallocs = 0;
		num_frees = 0;
	}
	return (t_malloc_pointers) {
		.ref_num_mallocs = &num_mallocs,
		.ref_num_frees = &num_frees,
		.ref_malloc_null_at = &malloc_null_at,
		.ref_malloc_random = &malloc_random,
		.ref_free_random = &free_random
	};
}

void my_get_real_functions(void* malloc_ptr, void* free_ptr)
{
	*(void**)malloc_ptr = original_malloc;
	*(void**)free_ptr = original_free;
}

// Nothing about the mallocing and freeing make sense, one malloc already has data, then i free it, malloc it again, still data, okay.. so far so good
// but the next malloc i make without freeing the previous one i randomize, free, and then all the data is ZEROD!? WHAT!?
void* my_malloc(size_t size)
{
	if (malloc_null_at == num_mallocs)
	{
		num_mallocs++;
		return NULL;
	}

	num_mallocs++;
	void* ptr = original_malloc(size);
	if (malloc_random)
		randomize_ptr(ptr, MALLOC_SIZE(ptr));
	return ptr;
}

void my_free(void* ptr)
{
	// in case something tries to free something that is inside the bootstrap malloc, the real free will error, so catch that here
	if (ptr >= (void*)&bootstrap_data[0] && ptr <= (void*)&bootstrap_data[bootstrap_size - 1])
		return;

	num_frees++;
	if (free_random)
		randomize_ptr(ptr, MALLOC_SIZE(ptr));
	original_free(ptr);
}

#pragma endregion

#pragma region override setup

__attribute__((constructor))
static void Constructor(int argc, const char **argv)
{
	is_bootstrapping = true;

	INIT_OVERRIDE(malloc)
	INIT_OVERRIDE(free)

	printf("%s","");
}

OVERRIDE(malloc)
OVERRIDE(free)
OVERRIDE(get_pointers)
OVERRIDE(get_real_functions)

#pragma endregion