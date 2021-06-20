#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	while (true)
	{
		char* read = readline("enter your command: ");
		printf("read line: %s\n", read);
		add_history(read);
		free(read);
	}
}