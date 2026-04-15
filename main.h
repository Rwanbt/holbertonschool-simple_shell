#ifndef SHELL_H
#define SHELL_H

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Prompt & Input */
void display_prompt(void);
int charread_input(void);

/* Parser */
char **parse_input(charinput);
void free_argv(char argv);

/* Path */
int charfind_command(char *command);
char get_path_dirs(void);

/* Executor */
int execute_command(char argv, char env);

/* Built-ins */
int is_builtin(charcommand);
int execute_builtin(char argv, char env);
int builtin_exit(char argv);
int builtin_env(char env);

/* Utils */
char *_strdup(char *str);
int _strlen(char *s);
char *_strcat(char *dest, char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);

#endif /* SHELL_H */
