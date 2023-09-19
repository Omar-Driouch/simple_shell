#include "shell.h"

/* <========================= Comment Handler =========================> */

/**
 * handle_commanets - Handle special command cases.
 * @cmd: An array of strings representing the command and its arguments.
 * @ex: A pointer to an integer representing the exit status.
 *
 * Return: 1 if the command was a special case, -2 otherwise.
 */
int handle_commanets(char **cmd, int **ex)
{

	if (_strcmp(cmd[0], "#") == 0)
	{
		**ex = 0;

		return (1);
	}

	return (-2);
}

/* <========================= Exit Handler =========================> */

/**
 * exit_ - Exit the custom shell with a specified status or error message.
 * @cmd: An array of strings representing the command and its arguments.
 * @l: An integer representing the current line number.
 *
 * Return: None (void).
 */
void exit_(char **cmd, int l)
{
	int status = 0;
	char error_message[] = "./hsh: 1: exit: Illegal number: ";

	if (cmd[1] != NULL)
	{
		if (_atoi(cmd[1]) < 0 || _strcmp(cmd[1], "HBTN") == 0)
		{
			write(STDERR_FILENO, error_message, _strlen(error_message));
			write(STDERR_FILENO, cmd[1], _strlen(cmd[1]));
			write(STDERR_FILENO, "\n", 1);
			status = 2;
		}
		if (_atoi(cmd[1]) > 98)
			status = 232;
		else if (_atoi(cmd[1]) > 0)
			status = atoi(cmd[1]);
		if (_atoi(cmd[1]) == 0 && cmd[1] != NULL)
			status = -1;
	}
	if (l > 2)
	{
		free_2d_array(cmd);
		exit(2);
	}

	if (status != -1)
	{
		free_2d_array(cmd);
		exit(status);
	}
	else
	{
		free_2d_array(cmd);
		exit(2);
	}
}

/* <========================= Semicolon Handler =========================> */

/**
 * semicolon_handler - Execute multiple commands separated by semicolons.
 * @cmd: An array of strings representing the semicolon-separated commands.
 * @ar: An array of strings representing the command-line arguments.
 * @env: An array of strings representing the environment variables.
 * @tok: A pointer to an integer representing the number of tokens.
 * @l: A pointer to an integer representing the current line number.
 * @ex: A pointer to an integer representing the exit status.
 * @lin: A pointer to a string representing the current line.
 *
 * Return: The exit status of the last executed command in the sequence.
 */
int semicolon_handler(char **cmd, char **ar, char **env, int *tok, int *l,
	int *ex, char *lin)
{
	int result = 0;
	char **cmdar = NULL;
	int i;

	(void)lin;

	free(lin);
	for (i = 0; cmd[i]; i++)
	{

		cmdar = (char **)malloc((2) * sizeof(char *));
		cmdar[0] = _strdup(cmd[i]);

		cmdar[1] = NULL;
		result = executecmd(cmdar, ar, env, tok, l, ex);
		*tok = *tok - 1;
	}
	free_2d_array(cmd);
	return (result);
}

/**
 * chech_semicolon - Check for the presence of semicolons
 * or ampersands in a string.
 * @source: A pointer to the string to be checked.
 *
 * Return: 1 if semicolons or ampersands are found, 0 otherwise.
 */
int chech_semicolon(char *source)
{
	int i;

	if (source == NULL)
	{
		return (0);
	}

	for (i = 0; source[i] != '\0'; i++)
	{
		if (source[i] == ';' || source[i] == '&')
			return (1);
	}

	return (0);
}

/* <================== Error Command Not Found Handler =================> */

/**
 * HndleErrorCmdNotfound - Handle the error for a command not found.
 * @arg: An array of strings representing the command-line arguments.
 * @cmd: An array of strings representing the command and its arguments.
 */
void HndleErrorCmdNotfound(char **arg, char **cmd)
{
	char error_message[] = ": not found";
	char *error = removeBin(cmd[0]);

	write(STDERR_FILENO, arg[0], _strlen(arg[0]));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, error, _strlen(error));
	write(STDERR_FILENO, error_message, _strlen(error_message));
	write(STDERR_FILENO, "\n", 1);
}

