/**
 * @file builtin.c
 * \brief Fonction Builtin shell
 * @author Pierre LEROUX
 * @version 1.0
 * @date 2017-04-14
 */

#include "fonc.h"
#include "builtin.h"

/* -----------------*/
/**
 * \brief Fonction de test Builtin
 * 			Affiche HELLO WORLD
 *
 * \param args Not Used
 *
 * \return  (0) OK
 */
/* -----------------*/
int bi_hello(char ***args){

	args=args;
	fprintf(stdout, "HELLO WORLD\n");
	fflush(stdout);

	return 0;
}

/* -----------------*/
/**
 * \brief Fonction de changement directory ENV
 *
 * \param args Le PATH
 *
 * \return (0) OK
 */
/* -----------------*/
int bi_cd(char ***args){

	args=args;
	fprintf(stdout, "cd bi_cd()\n");
	fflush(stdout);

	return 0;
}


