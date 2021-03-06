SRC = \
src/helpers.c \
src/env/env_get_set.c \
src/env/env_ptr.c \
src/executable/executable_log.c \
src/executable/executable_un_init.c \
src/executable/executable_add_here_doc.c \
src/executable/builtins/builtin_exit.c \
src/executable/builtins/builtin_unset.c \
src/executable/builtins/builtin_pwd.c \
src/executable/builtins/builtin_export.c \
src/executable/builtins/builtin_env.c \
src/executable/builtins/builtin_echo.c \
src/executable/builtins/executable_get_builtin_exec.c \
src/executable/builtins/builtin_cd.c \
src/executable/executable_add_pipe.c \
src/executable/executable_add_fd_redirect.c \
src/executable/executable_init.c \
src/executable/executable_add_arg.c \
src/executable/executable_run.c \
src/readline_ext/readline_ext.c \
src/utils/working_directory.c \
src/utils/path_utils.c \
src/utils/stupid_write.c \
src/utils/is_valid_env.c \
src/utils/find_executable.c \
src/malloc_wrappers/other.c \
src/malloc_wrappers/list.c \
src/read_and_execute/identify_string.c \
src/read_and_execute/3_cmd_to_blocks.c \
src/read_and_execute/3d_ambiguous_redirect.c \
src/read_and_execute/3a_invalid_grammar_rule.c \
src/read_and_execute/destroy.c \
src/read_and_execute/3c_expand_environment_variables.c \
src/read_and_execute/1_cmd_read_next.c \
src/read_and_execute/0_read_and_execute.c \
src/read_and_execute/5_run_execs.c \
src/read_and_execute/4_blocks_to_execs.c \
src/read_and_execute/3b_remove_quotes.c \
src/read_and_execute/2_cmd_split_in_spaces.c \
src/signal/controll_c_pressed.c \
src/signal/signal_handler.c \
