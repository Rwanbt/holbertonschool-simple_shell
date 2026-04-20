#include <stddef.h>
#include "main.h"

/**
 * is_builtin - checks if a command is a built-in
 * @command: the command to check
 * Return: 1 if the command is a built-in, 0 otherwise
 */
int is_builtin(char *command)
{
	if (!command)
		return (0);

	if (_strcmp(command, "exit") == 0 || _strcmp(command, "env") == 0)
		return (1);

	return (0);
}

/**
 * execute_builtin - executes a built-in command
 * @argv: the arguments for the built-in command
 * @env: the environment variables
 * Return: 0 on success, -1 on failure
 */
int execute_builtin(char **argv, char **env)
{
	if (_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv));

	if (_strcmp(argv[0], "env") == 0)
		return (builtin_env(env));

	return (-1);
}

/**
 * builtin_exit - exits the shell
 * @argv: the arguments for the exit command
 * Return: 0 on success, -1 on failure
 */
int builtin_exit(char **argv)
{
	(void)argv;
	return (0);
}

/**
 * builtin_env - prints the environment variables
 * @env: the environment variables
 * Return: 0 on success, -1 on failure
 */
int builtin_env(char **env)
{
	int i;

	if (!env)
		return (-1);

	for (i = 0; env[i] != NULL; i++)
		printf("%s\n", env[i]);

	return (0);
}

/**
 * dispatch_command - routes a command to the right handler
 * @argv: NULL-terminated argument vector (argv[0] is the command)
 * @env: environment variables array
 * @prog: program name (av[0] of main), forwarded to execute_command
 * @line: current command number, forwarded to execute_command
 *
 * Return: status of the command, or -1 if the command was "exit"
 */
int dispatch_command(char **argv, char **env, char *prog, int line)
{
	if (_strcmp(argv[0], "exit") == 0)
		return (-1);

	if (is_builtin(argv[0]))
		return (execute_builtin(argv, env));
	return (execute_command(argv, env, prog, line));
}
