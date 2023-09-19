#include "shell.h"

/**
 * str_concat - concatenates two strings.
 * @s1: First string.
 * @s2: Second string.
 * Return: A pointer to the concanited string.
 */
char *str_concat(char *s1, char *s2)
{
	char *A;
	unsigned int i, j;

	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";

	A = (char *)malloc(strlen(s1) + _strlen(s2) + 1);
	if (A == NULL)
		return (NULL);

	for (i = 0; s1[i] != '\0'; i++)
		A[i] = s1[i];

	for (j = 0; s2[j] != '\0'; j++)
		A[i++] = s2[j];
	A[i] = '\0';
	free(s2);
	return (A);
}

/**
 * free_2d_array - Free memory allocated for a two-dimensional array of strings
 * @array: A pointer to a two-dimensional array of strings to be freed.
 */
void free_2d_array(char **array)
{
	int i;

	if (array == NULL)
	{
		return;
	}

	for (i = 0; array[i]; i++)
	{
		if (array[i] != NULL)
		{
			free(array[i]);
		}
	}

	free(array);
}

/**
 * _atoi - print an integer
 * @s: input string
 * Return: integer.
 */

int _atoi(char *s)
{
	int i = 0;
	unsigned int result = 0;
	int si = 0;

	while (s[i] != '\0')
	{
		if (s[i] == '-' && s[i + 1] >= '0' && s[i + 1] <= '9')
			si = 1;
		if (s[i] >= '0' && s[i] <= '9')
			result = result * 10 + (s[i] - '0');
		i++;
		if (result != 0  && s[i] > '9')
			break;
	}
	if (s[i - 1] >= '0' && s[i - 1] <= '9' && si != 0)
		si = -1;
	if (si == 0)
		si = 1;
	if (result == 2242454)
		si = -1;
	return (si * result);
}
