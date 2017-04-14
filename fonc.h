/**
 * @file fonc.h
 * \brief Header include, fonction, extern
 * @author Pierre LEROUX
 * @version 1.0
 * @date 2017-03-28
 */


#ifndef FONC
#define FONC

/* Pour les constantes EXIT_SUCCESS et EXIT_FAILURE */
#include <stdlib.h>
/* Pour fprintf() */
#include <stdio.h>
/* Pour fork() */
#include <unistd.h>
/* Pour perror() et errno */
#include <errno.h>
/* Pour le type pid_t */
#include <sys/types.h>

#include <sys/wait.h>

#include <assert.h>

#include <string.h>

#include <signal.h>

#include <fcntl.h>

//

pid_t create_process(void);

void sh_quit();

void sh_prompt(void);

int sh_getLine(char **str);

int sh_getArgs(char **in, char ***out);

int sh_exec(char ***args);

int sh_parseCaractere(char *line);

int (* sh_searchFtExec(char ***args))(char***);

//

#define ALLOC_MEMORY 10



/* -----------------*/
/**
 * \brief Variable d'environnement glob
 */
/* -----------------*/
extern char **environ;

/* -----------------*/
/**
 * \brief Variable globale pid application
 */
/* -----------------*/

#endif
