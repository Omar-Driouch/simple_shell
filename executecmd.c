#include "shell.h"

/**
 * executecmd - Execute a command with specified arguments.
 * @cmd: An array of strings representing the command and its arguments.
 * @ar: An array of strings representing the command-line arguments.
 * @env: An array of strings representing the environment variables.
 * @tok: A pointer to an integer representing the number of tokens.
 * @l: A pointer to an integer representing the current line number.
 * @ex: A pointer to an integer representing the exit status.
 *
 * Return: The result of executing the command.
 */
int executecmd(char **cmd, char **ar, char **env, int *tok, int *l, int *ex)
{
	int cd = -2;

	if (_strcmp(cmd[0], "exit") == 0)
	{
		exit_(cmd, *l);
		return (1);
	}
	if (_strcmp(cmd[0], "clear") == 0)
	{
		_print_str("\033[H\033[J");
	}
	if (_strcmp(cmd[0], "mkdir") == 0)
	{
		if (mkdir(cmd[1], 0777) == -1)
			perror(cmd[0]);
		cd = 1;
	}
	cd = handle_commanets(cmd, &ex);
	if (_strcmp(cmd[0], "cd") == 0)
	{

		if (cd_command(*tok, cmd))
		{
			cd = -1;
		}
		else
		{
			cd = 0;
		}
	}
	return (executCMD(cmd, ar, env, &tok, cd));
}

/**
 * get_Home_Dir - Retrieve the home directory of the current user.
 * Return: A pointer to a string representing the user's home directory, or
 * NULL if the home directory is not found or an error occurs.
 */
char *get_Home_Dir(void)
{
	const char *directory_path = "/home";
	DIR *dir;
	struct dirent *entry;

	char *homedir = NULL;

	dir = opendir(directory_path);
	if (dir == NULL)
	{

		return (homedir);
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR)
		{
			if (_strcmp(entry->d_name, ".") != 0 && _strcmp(entry->d_name, "..") != 0)
			{
				homedir = entry->d_name;
			}
		}
	}

	closedir(dir);
	return (homedir);
}

/**
 * Home_navigate - Navigate to the user's home directory.
 * Return: 1 if successfully navigated to the home directory, 0 otherwise.
 */
int Home_navigate(void)
{

	const char *home_dir = my_getenv("HOME");

	if (!home_dir)
	{
		home_dir = my_getenv("PWD");
		if (!home_dir)
		{
			return (0);
		}
	}

	if (chdir(home_dir) != 0)
	{
		return (0);
	}
	return (1);
}

/* =================== Toknizer ======================= */

#include "shell.h"

/**
 * tokenizer - Tokenize and process input line into commands.
 * @line: A pointer to a pointer to the input line to be tokenized.
 * @tokens: A pointer to an integer to store the number of tokens.
 * @commands: A pointer to an array of strings to store the parsed commands.
 * @argv: A pointer to an array of strings representing program arguments.
 * @env: A pointer to an array of strings representing the environment.
 * @l: A pointer to an integer representing the line number.
 * @exi: A pointer to an integer representing the exit status.
 * @status: A pointer to an integer to store the final status.
 *
 * Return: The final status after command execution.
 */
int tokenizer(char **line, int *tokens, char **commands,
			  char **argv, char **env, int *l, int *exi, int *status)
{
	char *token = NULL, *temp = NULL, *lin = _strdup(*line);
	int cpt = 0, i = 0, chek = chech_semicolon(*line);

	if (!*line || !*line[0] || _strcmp(*line, "\n") == 0)
	{
		*tokens = 0;
		return (0);
	}
	temp = _strdup(*line);
	token = strtok(temp, DELIMITERS);
	while (token)
	{
		if ((_strcmp(token, "#") == 0 && i != 0) || _strcmp(token, "||") == 0)
			break;
		cpt++;
		token = strtok(NULL, DELIMITERS);
	}
	*tokens = cpt;
	commands = (char **)malloc(sizeof(char *) * (cpt + 1));
	token = strtok(*line, DELIMITERS);
	while (token)
	{
		if ((_strcmp(token, "#") == 0 && i != 0) || _strcmp(token, "||") == 0)
			break;
		commands[i] = _strdup(token);
		token = strtok(NULL, DELIMITERS);
		i++;
	}
	free(temp), free(*line);
	*line = NULL;
	commands[i] = NULL;
	if (!chek)
	{
		free(lin);
		*status = executecmd(commands, argv, env, tokens, l, exi);
	}
	else
		*status = semicolon_handler(commands, argv, env, tokens, l, exi, lin);
	return (*status);
}
