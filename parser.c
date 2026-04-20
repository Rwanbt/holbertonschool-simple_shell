#include "main.h"

/**
 * count_words - counts whitespace-separated words in a string
 * @line: the string to analyze
 *
 * Return: number of words
 */
static int count_words(char *line)
{
	int count = 0;
	int in_word = 0;
	int i = 0;

	while (line[i] != '\0')
	{
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

/**
 * parse_input - splits a line into a NULL-terminated argv array
 * @input: the line to split (modified by strtok)
 *
 * Return: malloc'd array of malloc'd words, or NULL on failure
 */
char **parse_input(char *input)
{
	char **argv;
	char *token;
	int n;
	int i = 0;

	n = count_words(input);
	argv = malloc(sizeof(char *) * (n + 1));
	if (argv == NULL)
		return (NULL);

	token = strtok(input, " \t\n");
	while (token != NULL)
	{
		argv[i] = _strdup(token);
		if (argv[i] == NULL)
		{
			free_argv(argv);
			return (NULL);
		}
		i++;
		token = strtok(NULL, " \t\n");
	}
	argv[i] = NULL;
	return (argv);
}

/**
 * free_argv - frees an argv array allocated by parse_input
 * @argv: the array to free (may be NULL)
 */
void free_argv(char **argv)
{
	int i = 0;

	if (argv == NULL)
		return;

	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
