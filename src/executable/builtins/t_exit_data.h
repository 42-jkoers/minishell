#ifndef T_EXIT_DATA_H
# define T_EXIT_DATA_H

typedef struct s_exit_data
{
	long long	exit_code;
	char		*print;
	bool		actually_exit;
}	t_exit_data;

#endif