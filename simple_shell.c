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

		line = read_input();
		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (*line == '\0')
		{
			free(line);
			continue;
		}

		argv = parse_input(line);
		free(line);
		if (argv == NULL || argv[0] == NULL)
		{
			free_argv(argv);
			continue;
		}

		status = dispatch_command(argv, environ);
		free_argv(argv);
		if (status == -1)
		{
			status = 0;
			break;
		}
	}
	return (status);
}
