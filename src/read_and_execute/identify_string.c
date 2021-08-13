#include "read_and_execute.h"
#include "minishell.h"

// WARNING: It is important that the longest grammar rule comes first in the
// array. Eg: "echo "a" >> x" matches ">" as seperator, when it should
// match ">>"
t_grammarinfo	get_grammar_rule_info(const char *str)
{
	if (!ft_strncmp(str, ">>", 2))
		return ((t_grammarinfo){B_DOUBLE_GREATER, 2});
	if (!ft_strncmp(str, "<<", 2))
		return ((t_grammarinfo){B_DOUBLE_LESSER, 2});
	if (!ft_strncmp(str, ">", 1))
		return ((t_grammarinfo){B_GREATER, 1});
	if (!ft_strncmp(str, "<", 1))
		return ((t_grammarinfo){B_LESSER, 1});
	if (!ft_strncmp(str, "|", 1))
		return ((t_grammarinfo){B_PIPE, 1});
	return ((t_grammarinfo){0, 0});
}

t_blocktype	type_quote(char c)
{
	if (c == '\'')
		return (B_SINGLE_QUOTE);
	if (c == '"')
		return (B_DOUBLE_QUOTE);
	return (0);
}
