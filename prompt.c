#include "main.h"

/**
 * display_prompt - prints the shell prompt to stdout
 *
 * Description: affiche "$ " UNIQUEMENT si l'entree standard est un
 * vrai terminal interactif. En mode non-interactif (ex: pipe avec
 * `echo "ls" | ./hsh` ou script), on n'affiche rien, pour que la
 * sortie reste propre et testable par le checker Holberton.
 */
void display_prompt(void)
{
	/* isatty(fd) renvoie 1 si fd est un terminal, 0 sinon         */
	/* STDIN_FILENO vaut 0 (constante pour le descripteur d'entree) */
	if (isatty(STDIN_FILENO))
	{
		/* write au lieu de printf : pas de buffer a flusher, */
		/* et plus simple pour une chaine courte              */
		write(STDOUT_FILENO, "$ ", 2);
	}
}

/**
 * read_input - reads one line of input from stdin
 *
 * Description: wrapper autour de getline. On laisse getline gerer
 * l'allocation (line=NULL, len=0 -> getline s'occupe de tout).
 * On nettoie le '\n' final car strtok le garderait sinon et execve
 * planterait en essayant d'executer "ls\n".
 *
 * Return: a malloc'd line without the trailing newline,
 *         or NULL on EOF / read error (Ctrl+D)
 */
char *read_input(void)
{
	/* line = NULL + len = 0 : on dit a getline de malloc elle-meme */
	/* getline va allouer / reallouer la taille necessaire          */
	char *line = NULL;
	size_t len = 0;

	/* ssize_t (et pas size_t) : getline peut renvoyer -1 sur EOF  */
	/* size_t est non-signe donc ne peut pas representer -1        */
	ssize_t nread;

	/* Lit une ligne complete (jusqu'au \n ou EOF)                 */
	/* Renvoie le nombre de caracteres lus, ou -1 si EOF/erreur    */
	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		/* Meme en cas d'EOF, getline a pu allouer un buffer   */
		/* Il faut le liberer sinon c'est une fuite memoire    */
		free(line);
		return (NULL);
	}

	/* Remplace le '\n' final par '\0' pour ne pas polluer execve  */
	if (line[nread - 1] == '\n')
		line[nread - 1] = '\0';
	return (line);
}
