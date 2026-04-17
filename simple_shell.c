#include "main.h"

/**
 * main - entry point of the simple shell
 *
 * Description: boucle REPL (Read - Eval - Print - Loop).
 * Tant qu'il y a des commandes à lire, on lit, on parse, on exécute,
 * on libère la mémoire et on recommence.
 *
 * Return: 0 on success, the status of the last command otherwise
 */
int main(void)
{
	/* line : la ligne brute lue par getline (malloc'd)              */
	/* argv : le tableau de mots après découpage (NULL-terminated)   */
	/* status : le code de retour de la derniere commande executee   */
	char *line;
	char **argv;
	int status = 0;

	while (1)
	{
		/* Affiche "$ " seulement si on est dans un vrai terminal  */
		display_prompt();

		/* Lit une ligne complete depuis stdin (avec getline)      */
		/* Renvoie NULL si EOF (Ctrl+D) ou erreur de lecture       */
		line = read_input();
		if (line == NULL)
		{
			/* En mode interactif, on saute une ligne apres le */
			/* Ctrl+D pour que le shell parent reprenne propre */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Si l'utilisateur a juste appuye sur Entree, on libere */
		/* la ligne vide et on affiche un nouveau prompt         */
		if (*line == '\0')
		{
			free(line);
			continue;
		}

		/* Decoupe la ligne en tableau de mots (argv)            */
		/* Chaque argv[i] est un strdup independant (malloc'd)   */
		argv = parse_input(line);

		/* Comme parse_input duplique chaque mot, on peut deja   */
		/* liberer 'line' des maintenant, on n'en a plus besoin  */
		free(line);

		/* Securite : si le malloc a echoue ou si argv est vide  */
		if (argv == NULL || argv[0] == NULL)
		{
			free_argv(argv);
			continue;
		}

		/* Fork + execve + wait (voir executor.c)                */
		/* On recupere le code de retour pour le return final    */
		status = execute_command(argv, environ);

		/* Liberation du tableau et de chaque mot qu'il contient */
		free_argv(argv);
	}
	return (status);
}
