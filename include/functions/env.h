#ifndef ENV_H
# define ENV_H

char*** env_ptr();

// copies a env pointer and sets that as the new env_ptr
void env_ptr_copy(char** envp);

char* env_get(char* name);
void env_set(char* name, char* new_value);

// Usefull env paths:
// PWD		Current working directory
// PATH		Find programs

#endif