#include "main.h"

/**
 * main - entry point of the simple shell
 *
 * Return: 0 on success, the status of the last command otherwise
 */
int main(void)
{
	char *line;
	char **argv;
	int status = 0;

	while (1)
	{
		display_prompt();

		/* Lit une ligne (NULL si Ctrl+D) */
		line = read_input();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Ligne vide -> nouveau prompt */
		if (*line == '\0')
		{
			free(line);
			continue;
		}

		/* Decoupe en argv, puis free line */
		argv = parse_input(line);
		free(line);
		if (argv == NULL || argv[0] == NULL)
		{
			free_argv(argv);
			continue;
		}

		/* Builtin : exit doit break, env et autres s'executent */
		if (is_builtin(argv[0]))
		{
			if (_strcmp(argv[0], "exit") == 0)
			{
				free_argv(argv);
				break;
			}
			status = execute_builtin(argv, environ);
		}
		else
		{
			/* Commande externe : fork + execve + wait */
			status = execute_command(argv, environ);
		}
		free_argv(argv);
	}
	return (status);
}
