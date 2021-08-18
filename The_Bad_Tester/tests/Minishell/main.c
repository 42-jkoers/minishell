#include "run_program.h"
#include "capture_fd.h"
#include "pid_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define RESET_COLOR "\e[0m"
#define BAD_INFO_COLOR "\e[31m"
#define BAD_LINE_COLOR "\e[38;5;88m"
#define GOOD_LINE_COLOR "\e[38;5;40m"
#define GOOD_INFO_COLOR "\e[38;5;83m"

static char* read_entire_fd(int fd)
{
	int buffer_size = 16;
	char* buffer = malloc(buffer_size * sizeof(char));
	int current = 0;

	//while (read(fd, &buffer[current], buffer_size - current) > 0)
	while (true)
	{
		int ret = read(fd, &buffer[current], buffer_size - current);
		//printf("at: %i/%i return: %i\n", current, buffer_size, ret);
		if (ret <= 0)
			break;
		current += ret;

		if (current == buffer_size)
		{
			buffer_size = buffer_size * 2;
			buffer = realloc(buffer, buffer_size  * sizeof(char));
		}
	}
	if (current >= buffer_size)
		buffer = realloc(buffer, (current + 1) * sizeof(char));
	buffer[current] = '\0';
	return buffer;
}

static int get_output(const char* executable_loc, const char* command, char** out, char** err)
{
	fflush(NULL);	// make sure that i dont read un-flushed stuffs in STDOUT

	t_capture_data STDIN = capture_fd(STDIN_FILENO, false);
	t_capture_data STDOUT = capture_fd(STDOUT_FILENO, true);
	t_capture_data STDERR = capture_fd(STDERR_FILENO, true);

	int pid = run_program_pid(1, executable_loc, NULL, NULL);
	write(STDIN.write_end, command, strlen(command));
	write(STDIN.write_end, "\nexit\n", 6);

	int status;
	waitpid(pid, &status, 0);
	int exit_code = get_exit_code(status);

	release_fd(&STDIN);
	release_fd(&STDOUT);
	release_fd(&STDERR);

	*out = read_entire_fd(STDOUT.read_end);
	*err = read_entire_fd(STDERR.read_end);

	free_captured_fd(&STDIN);
	free_captured_fd(&STDOUT);
	free_captured_fd(&STDERR);

	//printf("got out: %s\n", *out);
	//printf("got err: %s\n", *err);

	return exit_code;
}

static void print_start_of_discreprency(const char* command)
{
	printf(BAD_LINE_COLOR"for the command "BAD_INFO_COLOR"|%s|"BAD_LINE_COLOR" we have a discreprency!\n"RESET_COLOR, command);
}

static int count_occurances(const char* str, char chr)
{
	int count = 0;
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == chr)
			count++;
	}
	return count;
}

static char* filter_minishell_OUT_output(const char* command, const char* original_output)
{
	// minishell output also contains the entering of the text, while bash only has the real output
	// i need to filter out the minishell$ stuff, so thats what this is for
	size_t minishell_skip_count = strlen("minishell$ ") + strlen(command) +
		strlen(">") * count_occurances(command, '\n') +
		strlen("\n");
	size_t total_minishell_count = strlen(original_output);
	if (minishell_skip_count > total_minishell_count)
		minishell_skip_count = total_minishell_count;

	const char* remove_end_str = "minishell$ exit\n";
	//const char* remove_end_str = "minishell$ ";
	size_t remove_end_len = strlen(remove_end_str);
	if (total_minishell_count >= remove_end_len && !strcmp(&original_output[total_minishell_count - remove_end_len], remove_end_str))
		total_minishell_count -= remove_end_len;

	//printf("got output: |%s| vs: |%.*s|\n", bash_out, (int)total_minishell_count - minishell_skip_count, &minishell_out[minishell_skip_count]);

	char* new = malloc(total_minishell_count - minishell_skip_count + 1);
	memcpy(new, &original_output[minishell_skip_count], total_minishell_count - minishell_skip_count);
	new[total_minishell_count - minishell_skip_count] = '\0';

	//printf("Filtered STDOUT: |%s| > |%s|\n", original_output, new);
	return new;
}
static char* filter_minishell_ERR_output(const char* original_output)
{
	int minishell_skip_count = 0;

	size_t total_minishell_count = strlen(original_output);
	const char* remove_end_str = "exit\n";
	size_t remove_end_len = strlen(remove_end_str);
	if (total_minishell_count >= remove_end_len && !strcmp(&original_output[total_minishell_count - remove_end_len], remove_end_str))
		total_minishell_count -= remove_end_len;

	char* new = malloc(total_minishell_count - minishell_skip_count + 1);
	memcpy(new, &original_output[minishell_skip_count], total_minishell_count - minishell_skip_count);
	new[total_minishell_count - minishell_skip_count] = '\0';

	//printf("Filtered STDERR: |%s| > |%s|\n", original_output, new);
	return new;
}

static void remove_trailing_nl_if_both_have_it(char* a, char* b)
{
	int a_len = strlen(a);
	int b_len = strlen(b);

	if (a_len > 0 && b_len > 0 && a[a_len-1] == '\n' && b[b_len-1] == '\n')
	{
		a[a_len-1] = '\0';
		b[b_len-1] = '\0';
	}
}

static bool compare(const char* minishell_loc, const char* command)
{
	char* minishell_out;
	char* minishell_err;
	char* bash_out;
	char* bash_err;

	int minishell_exit_code = get_output(minishell_loc, command, &minishell_out, &minishell_err);
	int bash_exit_code = get_output("/bin/bash", command, &bash_out, &bash_err);

	char* filtered_minishell_out = filter_minishell_OUT_output(command, minishell_out);
	char* filtered_minishell_err = filter_minishell_ERR_output(minishell_err);

	remove_trailing_nl_if_both_have_it(filtered_minishell_out, bash_out);
	remove_trailing_nl_if_both_have_it(filtered_minishell_err, bash_err);

	//printf("got output: |%s| vs: |%s|\n", bash_out, filtered_minishell_out);
	if (strcmp(bash_out, filtered_minishell_out))
	{
		print_start_of_discreprency(command);
		printf(BAD_LINE_COLOR"\tExpected output in STDOUT: "BAD_INFO_COLOR"|%s|"BAD_LINE_COLOR", but got: "BAD_INFO_COLOR"|%s|"BAD_LINE_COLOR"\n"RESET_COLOR, bash_out, filtered_minishell_out);
		return false;
	}

	// I have no clue how bash does this
	//	exit 2>test puts exit in a file test
	// but running /bin/bash 2>test and then running exit does not put exit...
	// What??

	if (strcmp(bash_err, filtered_minishell_err))
	{
		print_start_of_discreprency(command);
		printf(BAD_LINE_COLOR"\tExpected output in STDERR: "BAD_INFO_COLOR"|%s|"BAD_LINE_COLOR", but got: "BAD_INFO_COLOR"|%s|"BAD_LINE_COLOR"\n"RESET_COLOR, bash_err, filtered_minishell_err);
		return false;
	}

	if (minishell_exit_code != bash_exit_code)
	{
		print_start_of_discreprency(command);
		printf(BAD_LINE_COLOR"\tExpected exit code: "BAD_INFO_COLOR"%i"BAD_LINE_COLOR", but got: "BAD_INFO_COLOR"%i"BAD_LINE_COLOR"\n"RESET_COLOR, bash_exit_code, minishell_exit_code);
		return false;
	}

	printf(GOOD_LINE_COLOR"Everything good for the command: "GOOD_INFO_COLOR"|%s|"GOOD_LINE_COLOR"!\n"RESET_COLOR, command);
	return true;
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: run_tests tests_file minishell_exec\n");
		return 1;
	}
	const char* tests_file_loc = argv[1];
	const char* minishell_exec_loc = argv[2];

	FILE *tests_file = fopen(tests_file_loc, "r");
	if (!tests_file)
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	struct stat sb;
	if (stat(tests_file_loc, &sb) == -1)
	{
		perror("stat");
		exit(EXIT_FAILURE);
	}

	char* file_contents = malloc(sb.st_size);
	char* current_file_contents = file_contents;

	add_fd_to_close_on_fork(tests_file->_fileno);

	int total_commands = 0;
	int good_commands = 0;
	while (fscanf(tests_file, "%[^\n] ", current_file_contents) != EOF)
	{
		if (ftell(tests_file) == -1)	// why, how are you still reading the entire file again, what even is happening, why does this break everything, how does it repeat? how does it effect the main process even tho its forked?
		{
			printf("\e[31mOi, something really wrong happened, what are you doing? currently at, well, i have no clue where im at anymore: %s\n", file_contents);
			break;
		}

		//printf("Got offset: %li and line: %s\n", ftell(tests_file), current_file_contents);
		size_t len = strlen(current_file_contents);
		if (current_file_contents[len-1] == '\\')
		{
			current_file_contents[len-1] = '\n';
			current_file_contents = &current_file_contents[len];
			continue;
		}

		if (compare(minishell_exec_loc, file_contents))
			good_commands++;
		current_file_contents = file_contents;
		total_commands++;
	}

	free(file_contents);
	remove_fd_from_close_on_fork(tests_file->_fileno);
	fclose(tests_file);

	printf("\e[0mtested %i different commands, %i passed (%i%%)!\n", total_commands, good_commands, good_commands * 100 / total_commands);
}