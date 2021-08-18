// So, we can use defines inside defines, and that makes this ALOT easier
// so, here are the defines:
// #define real_func_param_args > int x, int y
// #define real_func_fake_func > fake_func_name
// #define real_func_return_type > int
// #define real_func_call_args > x, y

// You can also use a X macro if you are overriding multiple functions at once

#ifndef __linux__
# define USE_INTERSPOSE
#endif

#define PRE_OVERRIDE(real_func)\
	real_func##_return_type (*original_##real_func)(real_func##_param_args) = NULL;

#define PRE_OVERRIDE_WITH_DEFAULT(real_func, default)\
	real_func##_return_type (*original_##real_func)(real_func##_param_args) = default;

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

# include <stdio.h>
# include <stdlib.h>

#ifdef USE_INTERSPOSE

// We need to use interpose
# define DYLD_INTERPOSE(_replacement,_replacee) \
	__attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
	__attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacement, (const void*)(unsigned long)&_replacee };

# define OVERRIDE(real_func)\
	DYLD_INTERPOSE(real_func##_fake_func, real_func)

# define INIT_OVERRIDE(real_func) original_##real_func = real_func; \
	if (original_##real_func == (void*)0) { fprintf(stderr, "Could not find real function "#real_func"!\nAdd corresponding #include"); exit(1); }

#else

// Used for dlsym, _GNU_SOURCE should be defined so that we have RTLD_NEXT
# include <dlfcn.h>

# ifndef RTLD_NEXT
#  error Either include override.h first or #define _GNU_SOURCE first
# endif

# define OVERRIDE(real_func)\
	real_func##_return_type real_func(real_func##_param_args) { \
		return real_func##_fake_func(real_func##_call_args); \
	}

# define INIT_OVERRIDE(real_func) original_##real_func = dlsym(RTLD_NEXT, #real_func); \
	if (original_##real_func == (void*)0) { fprintf(stderr, "Could not find real function "#real_func"!\n"); exit(1); }

#endif