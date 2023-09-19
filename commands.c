#include "shell.h"

/**
 * read_line - Read a line of input from standard input.
 * @line: A pointer to a pointer to a character buffer to store the input line.
 *
 * Return: 0 if the end of input is reached or an error occurs, 1 otherwise.
 */
int read_line(char **line)
{
	size_t len = 0;
	int n = 0;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);

	n = getline(line, &len, stdin);

	if (n == -1)
	{
		free(*line);
		*line = NULL;
		return (0);
	}

	if (n > 0 && (*line)[n - 1] == '\n')
		(*line)[n - 1] = '\0';

	return (isWhitespaceString(*line));
}

/**
 * removeBin - Remove the "/bin/" substring from a string.
 * @input: A pointer to the input string to be modified.
 *
 * Return: A pointer to the modified string with "/bin/" removed, or NULL if
 * the input string is empty.
 */
char *removeBin(char *input)
{
	char *start = input;
	char substring[] = "/bin/";
	char *source = input;
	size_t match = 0;

	while (*source)
	{
		if (*source == substring[match])
		{
			match++;
			source++;
			if (substring[match] == '\0')
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (match == strlen(substring))
	{
		while (*source)
		{
			*start++ = *source++;
		}
		*start = '\0';
	}

	if (*input)
	{
		return (input);
	}
	else
	{
		return (NULL);
	}
}

/**
 * executCMD - Execute a command with optional error handling.
 * @command: A pointer to an array of strings representing the command.
 * @argv: A pointer to an array of strings representing program arguments.
 * @envi: A pointer to an array of strings representing the environment.
 * @numstr: A pointer to an integer pointer to store the exit status.
 * @cd: An integer indicating the type of command execution
 *
 * Return: The exit status of the executed command.
 */
int executCMD(char **command, char **argv, char **envi, int **numstr, int cd)
{
	pid_t child = -1;
	int status = 0;

	(void)argv;
	if (cd == -1 || cd == 1 || cd == 0)
	{
		if (cd == 0)
			handleError_cd_(argv, command);
		**numstr = -1;
		free_2d_array(command);
		return (-2);
	}
	if (hasSubstring(command[0]) == 0)
	{
		command[0] = str_concat("/bin/", command[0]);
		if (!pathExists((command[0])))
			command[0] = removeBin(command[0]);
	}
	if (pathExists(command[0]))
	{
		child = fork();
		if (child == 0 && command[0] != NULL)
		{
			if (execve(command[0], command, envi) == -1)
			{
				free_2d_array(command);
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		HndleErrorCmdNotfound(argv, command);
		**numstr = -1, free_2d_array(command);
		return (0);
	}
	if (child != 0)
		waitpid(child, &status, 0);
	free_2d_array(command);
	return (WEXITSTATUS(status));
}

/**
 * processLine - Process a line of input and handle edge cases.
 *
 * @status: An integer representing the status of the line.
 * @line: A pointer to a pointer to the input line to be processed.
 *
 * Return: 0 if line should be freed and further processing skipped
 * 1 otherwise.
 */
int processLine(int status, char **line)
{
	if ((status == 0 || status == 1) && (*line[0] == '\0'))
	{
		free(*line);
		return (0);
	}
	if (status == 1)
	{
		free(*line);
		return (0);
	}
	return (1);
}

/**
 * pathExists - Check if a file or directory exists at the specified path.
 * @path: A pointer to a string containing the path to check for existence.
 *
 * Return: 1 if the file or directory exists at 'path', 0 otherwise.
 */
int pathExists(char *path)
{

	if (access(path, F_OK) == 0)
	{

		return (1);
	}
	else
	{

		return (0);
	}
}
