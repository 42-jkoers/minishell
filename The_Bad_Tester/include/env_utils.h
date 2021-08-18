#ifndef ENV_UTILS_H
# define ENV_UTILS_H

int get_env_indx(char** envp, char* name);
char** set_env(char** envp, char* name, char* value);
char** add_env(char** envp, char* name, char* value);
char** add_preload(char** envp, char* path_to_shared_lib);

#endif