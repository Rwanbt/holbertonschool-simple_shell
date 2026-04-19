#include "main.h"

/**
 * _build_path - Combines a directory and a command into a full path
 * @dir: The directory (e.g., /bin)
 * @command: The command (e.g., ls)
 *
 * Return: A newly allocated full path string, or NULL on failure
 */
char *_build_path(char *dir, char *command)
{
	char *full;
	int len = _strlen(dir) + _strlen(command) + 2;

	full = malloc(len);
	if (!full)
		return (NULL);

	_strcpy(full, dir);
	_strcat(full, "/");
	_strcat(full, command);
	return (full);
}

/**
 * _getenv - Get the value of an environment variable
 * @name: The name of the environment variable
 * @env: The environment variables array
 *
 * Return: A pointer to the value, or NULL if not found
 */
char *_getenv(const char *name, char **env)
{
	int i = 0, j;

	if (!name || !env)
		return (NULL);
	while (env[i])
	{
		j = 0;
		while (name[j] && name[j] == env[i][j])
			j++;
		if (name[j] == '\0' && env[i][j] == '=')
			return (env[i] + j + 1);
		i++;
	}
	return (NULL);
}

/**
 * find_command - Find the full path of a command
 * @command: The command to find
 * @env: The environment variables array
 *
 * Return: A pointer to the full path, or NULL if not found
 */
char *find_command(char *command, char **env)
{
	char *path_env, *path_copy, *dir, *full_path;
	struct stat st;

	if (!command)
		return (NULL);
	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (_strdup(command));
		return (NULL);
	}
	path_env = _getenv("PATH", env);
	if (!path_env || _strlen(path_env) == 0)
		return (NULL);
	path_copy = _strdup(path_env);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = _build_path(dir, command);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
