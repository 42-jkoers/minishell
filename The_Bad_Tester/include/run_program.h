#ifndef RUN_PROGRAM_H
# define RUN_PROGRAM_H

int run_program_pid(float timeout, const char* path, const char* const* argv, const char* const* envp);
int run_program(float timeout, const char* path, const char* const* argv, const char* const* envp);

#endif