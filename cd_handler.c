#include "shell.h"

/**
 * cd_command - Change the current working directory.
 *
 * @tokens: The number of tokens in the command.
 * @command: An array of strings representing the command and its arguments.
 *
 * Return: 1 if the directory change is successful, 0 otherwise.
 */
int cd_command(int tokens, char **command)
{
	char *prev_dir = my_getenv("OLDPWD"), *cwd;

	if (prev_dir == NULL && tokens == 2 && _strcmp(command[1], "-") == 0)
	{
		prev_dir = my_getenv("PWD");
		if (prev_dir)
			_print_str(prev_dir), _print_str("\n");
		return (1);
	}
	if (tokens == 1 || (tokens == 2 && _strcmp(command[1], "~") == 0))
	{
		return (Home_navigate());
	}
	else if (tokens == 2 && _strcmp(command[1], "-") == 0)
	{
		prev_dir = my_getenv("OLDPWD"), cwd = getcwd(NULL, 0);
		_print_str(prev_dir), _print_str("\n");
		if (chdir(prev_dir) != 0)
		{
			free(cwd);
			return (1);
		}
		set_variable_env("OLDPWD", cwd), free(cwd);
		return (1);
	}
	else if (tokens == 2)
	{
		cwd = getcwd(NULL, 0), set_variable_env("OLDPWD", cwd);
		if (chdir(command[1]) != 0)
		{
			free(cwd);
			return (0);
		}
		free(cwd);
	}
	else
		return (0);
	return (1);
}

/**
 * my_getenv - Retrieve the value of an environment variable.
 * @name: The name of the environment variable to retrieve.
 *
 * Return: A pointer to the value of the environment variable if found, or
 * NULL if the variable is not found or if the environment is NULL.
 */
char *my_getenv(char *name)
{
	int name_len = _strlen(name);
	char *result = NULL;
	char **env = environ;

	if (name == NULL || environ == NULL)
	{
		return (NULL);
	}

	while (*env != NULL)
	{
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
		{
			result = &(*env)[name_len + 1];
			break;
		}
		env++;
	}

	return (result);
}

/**
 * set_variable_env - Set or modify an environment variable.
 * @name: The name of the environment variable to set or modify.
 * @new_value: The new value to assign to the environment variable.
 *
 * Return: 1 on success, 0 if `name` is NULL or if `environ` is NULL.
 */
int set_variable_env(char *name, char *new_value)
{
	size_t name_len = _strlen(name);

	char **env = environ;
	int check = 0;

	if (name == NULL || environ == NULL)
	{
		return (0);
	}

	while (*env != NULL)
	{
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
		{

			check = 1;
			_strcpy(*env, "OLDPWD=");
			*env = _concat_whitout_malloc(*env, new_value);

			break;
		}

		env++;
	}

	if (!check)
	{
		add_OLDPWD_to_env("PWD", new_value);
	}

	return (1);
}

/**
 * add_OLDPWD_to_env - Add or modify the "OLDPWD" environment variable.
 * @name: The name of the environment variable to set or modify as "OLDPWD."
 * @new_value: The new value to assign to the "OLDPWD" environment variable.
 *
 * Return: 1 on success, 0 if `name` is NULL or if `environ` is NULL.
 */
int add_OLDPWD_to_env(char *name, char *new_value)
{
	int name_len = _strlen(name);

	char **env = environ;
	int check = 0;
	(void)new_value;
	if (name == NULL || environ == NULL)
	{
		return (0);
	}

	while (*env != NULL)
	{
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
		{

			check = 1;
			_strcpy(*env, "OLDPWD=");
			*env = _concat_whitout_malloc(*env, new_value);

			break;
		}

		env++;
	}
	if (!check)
	{
	}

	return (1);
}

/**
 * handleError_cd_ - Handle an error while changing the working directory.
 * @argv: An array of strings representing the command-line arguments.
 * @cmd: An array of strings representing the command and its arguments.
 */
void handleError_cd_(char **argv, char **cmd)
{
	char error_message[] = ": can't cd to ";
	char *error = cmd[0];

	write(STDERR_FILENO, argv[0], _strlen(argv[0]));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, error, _strlen(error));
	write(STDERR_FILENO, error_message, _strlen(error_message));
	write(STDERR_FILENO, cmd[1], _strlen(cmd[1]));
	write(STDERR_FILENO, "\n", 1);
}
