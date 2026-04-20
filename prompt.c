#include "main.h"

/**
 * display_prompt - prints "$ " if stdin is a terminal
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);
}

/**
 * read_input - reads one line from stdin using getline
 *
 * Return: a malloc'd line without the trailing '\n',
 *         or NULL on EOF / error
 */
char *read_input(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}

	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';
	return (line);
}
