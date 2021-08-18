#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int get_env_indx(char** envp, char* name)
{
	int i = 0;
	int len = strlen(name);

	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return i;
		i++;
	}
	return -1;
}

// Dont care about leaks
char** set_env(char** envp, char* name, char* value)
{
	// Count num_env_values
	int num_env_values;
	for (num_env_values = 0; envp[num_env_values]; num_env_values++);

	// Create new envp
	int index = get_env_indx(envp, name);
	int new_num_env_values = num_env_values + (index == -1 ? 1 : 0);
	char** new_envp = malloc(sizeof(char*) * (new_num_env_values + 1));
	memcpy(new_envp, envp, sizeof(char*) * num_env_values);
	new_envp[new_num_env_values] = NULL;

	// Create new string
	char* new_value_str = malloc(strlen(name) + strlen(value) + 2);
	memcpy(new_value_str, name, strlen(name));
	new_value_str[strlen(name)] = '=';
	memcpy(new_value_str + strlen(name) + 1, value, strlen(value));

	// Insert
	int insert_index = index == -1 ? num_env_values : index;
	new_envp[insert_index] = new_value_str;

	return new_envp;
}

// Dont care about leaks
// Same as set_end, but if it already exists, append it to the end
char** add_env(char** envp, char* name, char* value)
{
	int index = get_env_indx(envp, name);
	if (index == -1)
		return set_env(envp, name, value);
	else
	{
		char* prev = envp[index];

		// Check if it is already in the env
		char* eq = strchr(prev, '=');
		int value_len = strlen(value);
		while (eq)
		{
			eq++;
			if (strncmp(eq, value, value_len) == 0 && (eq[value_len] == '\0' || eq[value_len] == ':'))
				return envp;	// already added
			eq = strchr(prev, ':');
		}

		// Append to new_value_str
		char* new_value_str = malloc(strlen(prev) + value_len + 2);
		memcpy(new_value_str, prev, strlen(prev));
		new_value_str[strlen(prev)] = ':';
		memcpy(new_value_str + strlen(prev) + 1, value, value_len);

		// set
		return set_env(envp, name, new_value_str);
	}
}

// Dont care about leaks
char** add_preload(char** envp, char* path_to_shared_lib)
{
	#if __linux__
	return add_env(envp, "LD_PRELOAD", path_to_shared_lib);
	#else
	char** new_env = add_env(envp, "DYLD_INSERT_LIBRARIES", path_to_shared_lib);
	return set_env(new_env, "DYLD_FORCE_FLAT_NAMESPACE", "1");
	#endif
}