#include "main.h"

/**
 * count_words - counts the number of whitespace-separated words
 * @line: the string to analyze
 *
 * Description: machine a etat simple pour compter les mots.
 * On parcourt la chaine caractere par caractere et on change d'etat
 * (dans un mot / hors d'un mot) a chaque transition. On incremente
 * le compteur uniquement au debut de chaque nouveau mot.
 *
 * 'static' : cette fonction n'est utilisee que dans parser.c,
 * donc on la rend privee au fichier (pas visible depuis l'exterieur).
 *
 * Return: number of words
 */
static int count_words(char *line)
{
	int count = 0;
	/* in_word : 1 si on est en train de lire un mot, 0 sinon     */
	int in_word = 0;
	int i = 0;

	while (line[i] != '\0')
	{
		/* Si on tombe sur un separateur -> on sort du mot     */
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
		{
			in_word = 0;
		}
		/* Si on etait HORS d'un mot et qu'on voit un char normal, */
		/* c'est qu'un nouveau mot commence -> on incremente       */
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		/* Si on etait deja dans un mot, on continue sans rien faire */
		i++;
	}
	return (count);
}

/**
 * parse_input - splits an input line into a NULL-terminated argv array
 * @input: the line to split (will be modified by strtok)
 *
 * Description: transforme une ligne texte en tableau de mots utilisable
 * comme argv par execve. Etapes :
 *   1. compter les mots pour savoir combien de pointeurs allouer
 *   2. malloc le tableau (n+1 pour le NULL final)
 *   3. strtok pour extraire chaque mot, puis _strdup pour le copier
 *   4. NULL-terminate le tableau
 *
 * Pourquoi _strdup et pas juste pointer dans 'input' ?
 * Parce que strtok modifie 'input' en remplacant les separateurs
 * par '\0'. Si argv[i] pointait dans input, on serait oblige de
 * garder 'input' en vie tant qu'on utilise argv. En dupliquant
 * chaque mot, argv devient totalement independant de input, ce qui
 * simplifie la gestion memoire dans main (on peut free(line) tout
 * de suite apres parse_input).
 *
 * Return: a malloc'd array of malloc'd words, or NULL on failure.
 *         Each argv[i] is an independent malloc'd string.
 */
char **parse_input(char *input)
{
	char **argv;
	char *token;
	int n;
	int i = 0;

	/* Etape 1 : on compte les mots pour dimensionner le tableau   */
	n = count_words(input);

	/* Etape 2 : on alloue n+1 pointeurs (le +1 pour le NULL final */
	/* qui sert de marqueur de fin, comme dans argv de main)       */
	argv = malloc(sizeof(char *) * (n + 1));
	if (argv == NULL)
		return (NULL);

	/* Etape 3 : premier appel a strtok avec la chaine a decouper  */
	/* Les delimiteurs sont espace, tab, retour a la ligne         */
	token = strtok(input, " \t\n");
	while (token != NULL)
	{
		/* Duplication independante de chaque token               */
		argv[i] = _strdup(token);
		if (argv[i] == NULL)
		{
			/* Si _strdup echoue, on nettoie ce qu'on avait   */
			/* deja alloue pour eviter une fuite memoire      */
			free_argv(argv);
			return (NULL);
		}
		i++;
		/* Les appels suivants a strtok passent NULL pour dire  */
		/* "continue sur la meme chaine que la derniere fois"   */
		token = strtok(NULL, " \t\n");
	}

	/* Etape 4 : NULL final, obligatoire pour que execve sache */
	/* ou s'arrete la liste d'arguments                         */
	argv[i] = NULL;
	return (argv);
}

/**
 * free_argv - frees an argv array allocated by parse_input
 * @argv: the array to free (may be NULL)
 *
 * Description: libere d'abord chaque mot (argv[i]), PUIS le tableau
 * lui-meme. Ordre important : si on freeait argv avant ses elements,
 * on perdrait les pointeurs vers chaque mot et on aurait une fuite.
 */
void free_argv(char **argv)
{
	int i = 0;

	/* Securite : si on appelle free_argv(NULL), ne rien faire     */
	if (argv == NULL)
		return;

	/* Parcourt jusqu'au NULL terminal et free chaque mot           */
	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}
	/* Une fois tous les mots liberes, on libere le tableau        */
	free(argv);
}
