#include "shell.h"

/**
 * sigint_handler - Handles the SIGINT signal.
 */
void sigint_handler(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n$ ", 3);
}

/**
 * main - Entry point for the custom shell program.
 * @ac: The number of command-line arguments.
 * @argv: An array of strings representing the command-line arguments.
 *
 * Return: The exit status of the shell program.
 */
int main(int ac, char **argv)
{
	char *line = NULL;
	char **command = NULL;
	int status = 0, tokens = 0;
	int l = 1, exi = 127;

	if (signal(SIGINT, (void (*)(int))sigint_handler) == SIG_ERR)
		return (1);

	(void)ac;
	while (l)
	{
		status = read_line(&line);
		l++;
		if (!line)
		{
			if (tokens == -1)
				exit(exi);
			exit(status);
		}
		if (!processLine(status, &line))
			continue;
		status = tokenizer(&line, &tokens, command, argv, environ, &l, &exi,
			&status);

		if (!command && line == NULL)
		{
			free_2d_array(command);
			continue;
		}
		if (status == 1)
		{
			free_2d_array(command);
			continue;
		}
	}
	exit(0);
}
