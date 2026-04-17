#include "main.h"

/**
 * execute_command - forks and executes a command with its arguments
 * @argv: NULL-terminated argument vector (argv[0] is the command name)
 * @env: environment to pass to the executed program
 *
 * Description: coeur de l'execution d'une commande. Etapes :
 *   1. find_command pour obtenir le chemin complet (ex: "ls" -> "/bin/ls")
 *   2. fork pour creer un processus fils
 *   3. dans le fils : execve (remplace l'image par la commande)
 *   4. dans le pere : wait + recuperer le code de retour
 *   5. free et return
 *
 * Return: exit status of the child, or 127 if the command is not found
 */
int execute_command(char **argv, char **env)
{
	char *full_path;
	pid_t pid;
	int status = 0;

	/* Etape 1 : chercher le chemin complet de la commande          */
	full_path = find_command(argv[0]);
	if (full_path == NULL)
	{
		/* Commande introuvable -> message d'erreur sur stderr */
		/* et on retourne 127 (convention Unix standard)       */
		fprintf(stderr, "%s: command not found\n", argv[0]);
		return (127);
	}

	/* Etape 2 : fork duplique le processus                          */
	/* - renvoie le PID du fils au pere                              */
	/* - renvoie 0 dans le fils                                      */
	/* - renvoie -1 en cas d'erreur                                  */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full_path);
		return (1);
	}

	/* Etape 3 : dans le fils (pid == 0), on lance execve            */
	/* execve REMPLACE l'image memoire du fils par la commande.      */
	/* Si execve reussit, le code apres n'est JAMAIS execute.        */
	/* Si execve echoue, il faut perror et exit, sinon le fils va    */
	/* continuer a tourner comme un shell, ce qui est le bazar.      */
	if (pid == 0)
	{
		if (execve(full_path, argv, env) == -1)
		{
			perror(argv[0]);
			free(full_path);
			exit(127);
		}
	}
	/* Etape 4 : dans le pere, on attend la fin du fils              */
	else
	{
		/* wait(&status) bloque jusqu'a la fin du fils et remplit */
		/* status avec un entier encode (pas directement le code) */
		wait(&status);

		/* WIFEXITED : le fils s'est termine normalement ?        */
		/* WEXITSTATUS : extrait le vrai code de retour (0-255)   */
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
	}

	/* Etape 5 : liberation (dans le pere uniquement, le fils est   */
	/* soit replace par execve, soit deja exit)                     */
	free(full_path);
	return (status);
}
