#ifndef AUTO_SNPRINTF_H
# define AUTO_SNPRINTF_H

# include <stddef.h>
# include <stdarg.h>

typedef struct s_auto_sprintf
{
	int length;
	char* buffer;
}	t_auto_sprintf;

// Cheeky define because i dont feel like writing this out constantly when i want to make a new t_auto_sprintf
#define def_auto_sprintf (t_auto_sprintf) { 0, NULL }

char* tmp_vsprintf(t_auto_sprintf* data, char* format, va_list ap);
char* auto_vsprintf(char* format, va_list ap);
char* tmp_sprintf(t_auto_sprintf* data, char* format, ...);
char* auto_sprintf(char* format, ...);

#endif