#include <stddef.h>
#include "main.h"

/**
 * _strlen - returns the length of a string
 * @s: the string to check
 * Return: the length of the string
 */
int _strlen(char *s)
{
	int len = 0;

	while (s[len])
		len++;
	return (len);
}

/**
 * _strcpy - copies a string
 * @dest: the destination string
 * @src: the source string
 * Return: the pointer to dest
 */
char *_strcpy(char *dest, char *src)
{
	int i;

	for (i = 0; src[i]; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

/**
 * _strcmp - compares two strings
 * @s1: the first string
 * @s2: the second string
 * Return: 0 if the strings are equal,
 * a positive number if s1 is greater than s2,
 * or a negative number if s1 is less than s2
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	for (i = 0; s1[i] && s2[i]; i++)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
	}
	return (s1[i] - s2[i]);
}

/**
 * _strcat - concatenates two strings
 * @dest: the destination string
 * @src: the source string
 * Return: the pointer to dest
 */
char *_strcat(char *dest, char *src)
{
	int dest_len = _strlen(dest);
	int i;

	for (i = 0; src[i]; i++)
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';
	return (dest);
}

/**
 * char_strdup - duplicates a string
 * @str: the string to duplicate
 * Return: a pointer to the duplicated string,
 * or NULL if memory allocation fails
 */
char *char_strdup(char *str)
{
	char *dup;
	int len = _strlen(str);

	dup = malloc(sizeof(char) * (len + 1));

	if (!dup)
	return (NULL);
	_strcpy(dup, str);
	return (dup);
}
