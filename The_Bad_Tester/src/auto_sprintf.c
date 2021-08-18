#include "auto_sprintf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char* tmp_vsprintf(t_auto_sprintf* auto_data, char* format, va_list ap)
{
	if (auto_data->length < 16)
	{
		free(auto_data->buffer);
		auto_data->buffer = 0;
		auto_data->length = 16;
	}

	if (auto_data->buffer == NULL)
		auto_data->buffer = malloc(auto_data->length);

	va_list cpy;
	while (1)
	{
		va_copy(cpy, ap);
		int len = vsnprintf(auto_data->buffer, auto_data->length, format, cpy);
		va_end(cpy);

		if (len < auto_data->length)
			return auto_data->buffer;
		auto_data->length *= 2;
		auto_data->buffer = realloc(auto_data->buffer, auto_data->length);
	}
}

char* auto_vsprintf(char* format, va_list ap)
{
	t_auto_sprintf auto_data = def_auto_sprintf;
	return tmp_vsprintf(&auto_data, format, ap);
}

char* tmp_sprintf(t_auto_sprintf* auto_data, char* format, ...)
{
	va_list lst;
	va_start(lst, format);
	char* ret = tmp_vsprintf(auto_data, format, lst);
	va_end(lst);
	return ret;
}

char* auto_sprintf(char* format, ...)
{
	va_list lst;
	va_start(lst, format);
	char* ret = auto_vsprintf(format, lst);
	va_end(lst);
	return ret;
}
