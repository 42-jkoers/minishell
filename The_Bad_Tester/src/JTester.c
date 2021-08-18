#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "JTester.h"
#include "test_utils_int.h"

#define MAX_TEST_NAME_LEN 15

// Mhh, kinda annoying that i have to add the bind lib.. :/
extern void get_real_functions(void* malloc_ptr, void* free_ptr);
void* (*real_malloc)(size_t size) = malloc;
void (*real_free)(void* ptr) = free;

int NUM_OK     = 0;
int NUM_KO     = 0;
int NUM_WARN   = 0;
int NUM_SIG_OK = 0;
int NUM_SIG_KO = 0;
int NUM_NEW    = 0;

static int TRUE_STDOUT_FILENO;
void JTester_init()
{
	test_message_init();

	TRUE_STDOUT_FILENO = dup(STDOUT_FILENO);	// In case we capture STDOUT, we have a duplicate here, so we will always write to stdout

	get_real_functions(&real_malloc, &real_free);
}

char* read_fd_completely(int fd, int* num_chars)
{
	char* buff = real_malloc(16);	// use real_malloc to not mess with the malloc count
	int buff_size = 16;

	*num_chars = 0;
	while (1)
	{
		int read_bytes = read(fd, buff + *num_chars, buff_size - *num_chars);
		if (read_bytes == -1)
			break;
		*num_chars += read_bytes;
		if (*num_chars != buff_size)
			break;
		char* new_buff = real_malloc(buff_size * 2);
		memcpy(new_buff, buff, *num_chars);
		buff_size *= 2;
		real_free(buff);
		buff = new_buff;
	}
	return buff;
}

typedef enum e_message_type
{
	OK,
	KO,
	WARN,
	SIG_OK,
	SIG_KO,
	NEW,
	UNKNOWN
}	message_type;

void JTester_parse_string(char* string, int length)
{
	int typelen = -1;
	message_type type = UNKNOWN;

	if (     strncmp(string, "OK",     2) == 0) { NUM_OK    ++; type = OK;     typelen = 2; }
	else if (strncmp(string, "KO",     2) == 0) { NUM_KO    ++; type = KO;     typelen = 2; }
	else if (strncmp(string, "WARN",   4) == 0) { NUM_WARN  ++; type = WARN;   typelen = 4; }
	else if (strncmp(string, "SIG_OK", 6) == 0) { NUM_SIG_OK++; type = SIG_OK; typelen = 6; }
	else if (strncmp(string, "SIG_KO", 6) == 0) { NUM_SIG_KO++; type = SIG_KO; typelen = 6; }
	else if (strncmp(string, "NEW",    3) == 0) { NUM_NEW   ++; type = NEW;    typelen = 3; }
	//printf("Got string: %.*s\n", length, string);
	if (type == NEW)
	{
		static bool first_time = true;
		if (first_time)
			first_time = false;
		else
			printf("\n");	// print a \n if this is not the first test
		int num_spaces = MAX_TEST_NAME_LEN - (length - typelen);
		if (num_spaces < 0)
			num_spaces = 1;
		printf(CL_COL_TEST"%.*s:%*s"CL_COL_DEF, length - typelen, &string[typelen], num_spaces, "");
	}
	else if (type == OK)
		printf(CL_COL_OK"[%.*s]"CL_COL_DEF, length - typelen, &string[typelen]);
	else if (type == KO)
		printf(CL_COL_KO"[%.*s]"CL_COL_DEF, length - typelen, &string[typelen]);
	else if (type == WARN)
		printf(CL_COL_WARN"[%.*s]"CL_COL_DEF, length - typelen, &string[typelen]);
	else if (type == SIG_OK)
		printf(CL_COL_OK"[%.*s]"CL_COL_DEF, length - typelen, &string[typelen]);
	else if (type == SIG_KO)
		printf(CL_COL_KO"[%.*s]"CL_COL_DEF, length - typelen, &string[typelen]);
	else
		printf(CL_COL_ERR"[Got string: %.*s]"CL_COL_DEF, length, string);
	//fflush(NULL);
}

void JTester_flush()
{
	if (WOULD_CATCH)	// dont do that
		return;

	int num_chars;
	char* string = read_fd_completely(MESSAGE_READ_FD, &num_chars);

	int start = 0;
	int curr = 0;
	while (curr < num_chars)
	{
		if (string[curr] == SPLIT_CHAR)
		{
			JTester_parse_string(&string[start], curr - start);
			start = curr + 1;
		}
		curr++;
	}

	real_free(string);
}

void JTester_exit()
{
	JTester_flush();

	close(MESSAGE_WRITE_FD);
	close(MESSAGE_READ_FD);
	if (NUM_NEW != 0)
		printf("\n");

	if (NUM_KO > 0 || NUM_SIG_KO > 0)
		exit(1);
	else
		exit(0);
}