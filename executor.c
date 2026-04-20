#include "main.h"

/**
 * execute_command - forks and executes a command
 * @argv: NULL-terminated argument vector (argv[0] is the command)
 * @env: environment to pass to the executed program
 *
 * Return: exit status of the child, or 127 if command not found
 */
int execute_command(char **argv, char **env)
{
	char *full_path;
	pid_t pid;
	int status = 0;

	full_path = find_command(argv[0], env);
	if (full_path == NULL)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full_path);
		return (1);
	}

	if (pid == 0)
	{
		if (execve(full_path, argv, env) == -1)
		{
			perror(argv[0]);
			free(full_path);
			exit(127);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}

	free(full_path);
	return (status);
}
