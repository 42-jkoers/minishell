#include <stdio.h>
#include "env.h"
#include "read_and_execute.h"

void	print_blocks(const t_list *blocks)
{
	size_t	i;
	t_block	*block;

	i = 0;
	printf("blocks            ");
	while (i < blocks->count)
	{
		block = list_index_unchecked(blocks, i);
		printf("`%s`", block->text);
		if (i + 1 != blocks->count)
			printf(", ");
		i++;
	}
	printf("\n");
}

void	print_split_in_spaces(const t_list *split)
{
	size_t	i;
	char	*block;

	i = 0;
	printf("split_in_spaces   ");
	while (i < split->count)
	{
		block = *(char **)list_index_unchecked(split, i);
		printf("`%s`", block);
		if (i + 1 != split->count)
			printf(", ");
		i++;
	}
	printf("\n");
}

int	main(int argc, char **argv, const char **envp)
{
	char	*raw_string;
	t_list	split_in_spaces;
	t_list	blocks;

	(void)argc;
	(void)argv;
	env_copy_ptr(envp);
	while (true)
	{
		raw_string = cmd_read_next();
		printf("\n");
		if (!cmd_split_in_spaces(&split_in_spaces, raw_string))
			continue ;
		print_split_in_spaces(&split_in_spaces);
		if (!cmd_to_blocks(&blocks, &split_in_spaces))
			continue ;
		print_blocks(&blocks);
		printf("\n");
	}
}
