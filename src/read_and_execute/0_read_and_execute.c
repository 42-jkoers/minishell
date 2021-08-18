#include <stdlib.h>

#include "env.h"
#include "read_and_execute.h"
#include "malloc_wrappers.h"
#include "libft.h"
#include "signal_handler.h"
#include "minishell.h"
#include "executable.h"

typedef struct s_read_exec
{
	char		*raw_string;
	t_list		split_in_spaces;
	t_list		blocks;
	t_list		execs;
	t_exitcode	exitcode;
}		t_read_exec;

static int	destructor(t_read_exec *r)
{
	if (r->raw_string)
		free(r->raw_string);
	if (r->split_in_spaces.count)
		list_un_init_ptr(&r->split_in_spaces);
	if (r->blocks.count)
		free_blocks(&r->blocks);
	if (r->execs.count)
		list_un_init(&r->execs, (t_free_values)executable_un_init);
	return (r->exitcode);
}

// TODO: return -1?
t_exitcode	read_and_execute_command(void)
{
	t_read_exec	r;

	ft_bzero(&r, sizeof(t_read_exec));
	r.raw_string = cmd_read_next();
	if (!r.exitcode)
		r.exitcode = cmd_split_in_spaces(&r.split_in_spaces, r.raw_string);
	if (!r.exitcode)
		r.exitcode = cmd_to_blocks(&r.blocks, &r.split_in_spaces);
	if (!r.exitcode)
		r.exitcode = blocks_to_execs(&r.execs, &r.blocks);
	if (!r.exitcode)
		r.exitcode = run_execs(&r.execs);
	return (destructor(&r));
}
