#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "JTester.h"
#include "test_utils_int.h"

int MESSAGE_WRITE_FD = -1;
int MESSAGE_READ_FD = -1;

void test_message_init()
{
	int pipefd[2];
	pipe(pipefd);

	fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
	fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

	MESSAGE_READ_FD = pipefd[0];
	MESSAGE_WRITE_FD = pipefd[1];
}

static void write_to_test_output_fd(char* message)
{
	write(MESSAGE_WRITE_FD, message, strlen(message));
}

static void test_message(char* type, char* message)
{
	write_to_test_output_fd(type);
	write_to_test_output_fd(message);
	write_to_test_output_fd(SPLIT_STR);

	JTester_flush();
}

void test_new(char* test_name)
{
	test_message("NEW", test_name);
}

void test_ok(char* message)
{
	if (message == NULL || message[0] == '\0')
		test_message("OK", "OK");
	else
		test_message("OK", message);
}

void test_ko(char* message)
{
	if (message == NULL || message[0] == '\0')
		test_message("KO", "KO");
	else
		test_message("KO", message);
}

void test_warning(char* message)
{
	if (message == NULL || message[0] == '\0')
		test_message("WARN", "WARN");
	else
		test_message("WARN", message);
}

void test_sig(char* message, bool good_sig)
{
	char* type = good_sig ? "SIG_OK" : "SIG_KO";
	if (message == NULL || message[0] == '\0')
		test_message(type, "SIG");
	else
		test_message(type, message);
}