#include <string.h>
#include <stdlib.h>

#include <JTester.h>

// free returned pointer after you are done with it
char* get_basename(char* path)
{
	int len = strlen(path);
	int curr = len;
	while (curr > 0)
	{
		if (path[curr] == '/')
		{
			char* new = malloc(curr + 2);
			memcpy(new, path, curr + 1);
			new[curr + 1] = '\0';
			return new;
		}
		curr--;
	}

	// We dont have a /...
	char* new = malloc(len + 2);
	memcpy(new, path, len);	// its just a folder name or something
	memcpy(new + len, "/", 2);	// add a / at the end, to indicate that its a folder
	return new;
}

// Does not malloc, simply returns pointer inside path given
char* get_filename(char* path)
{
	char* found = strrchr(path, '/');
	if (found)
		return found + 1;
	return path;
}

char* path_join(char* start, char* end)
{
	// Make sure startlen does not contain a / at the end
	int startlen = strlen(start);
	if (start[startlen - 1] == '/')
		startlen--;

	// Make sure endlen does not contain a / at the start
	if (end[0] == '/')
		end++;
	int endlen = strlen(end);

	char* new = malloc(startlen + 1 + endlen + 1);
	memcpy(new, start, startlen);
	new[startlen] = '/';
	memcpy(new + startlen + 1, end, endlen + 1);
	return new;
}

char* path_relative(char* path)
{
	return path_join(COMPILE_PATH, path);
}