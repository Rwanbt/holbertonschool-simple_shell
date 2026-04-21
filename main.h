#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Prompt & Input */
void display_prompt(void);
char *read_input(void);

/* Parser */
char **parse_input(char *input);
void free_argv(char **argv);

/* Path */
char *_build_path(char *dir, char *command);
char *_getenv(const char *name, char **env);
char *find_command(char *command, char **env);

/* Executor */
int execute_command(char **argv, char **env, char *prog, int line);

/* Built-ins */
int is_builtin(char *command);
int execute_builtin(char **argv, char **env);
int builtin_exit(char **argv);
int builtin_env(char **env);
int dispatch_command(char **argv, char **env, char *prog, int line);

/* Utils */
char *_strdup(char *str);
int _strlen(char *s);
char *_strcat(char *dest, char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);

#endif
