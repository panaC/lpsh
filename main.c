/**
 * \file  main.c
 * \brief mini shell implémentation in C
 * \author pierre
 * \date 28/03/2017
 */

/* Copyright (C) 
 * 2017 - Pierre LEROUX
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * ))
 */

#include "fonc.h"

/**
 * \brief Point d'entrée application
 *
 * \param argc Nombre d'argument en parametre
 * \param argv Tableau de string contenant les arguments
 *
 * \return EXIT_SUCCESS ok
 * \return EXIT_FAILURE ko
 */
int main(int argc, char **argv)
{
	argc=argc; //Not used
	argv=argv; //Not used

	char *line;
	char **argArray;
	char *tstr = NULL;
	char tmp[3];

	int retExec = 0;
	int nbAllocMemoryLine = 1;
	int lenLine = 0;
	int b_notAction = 0;
	int b_exit = 0;

	int dup_in = dup(STDIN_FILENO);

	line = NULL;
	argArray = NULL;
	errno = 0;

	signal(SIGINT, sh_quit); 

	//Alloue mémoire pour ajout caractere (INIT)
	line = (char*) calloc(ALLOC_MEMORY, sizeof(char));
		
	while(1) {
		if(b_exit){
			break;
		}
	
		sh_prompt();
		line[0] = '\0';
		b_notAction = 1;

		while(b_notAction){

			lenLine = strlen(line);
			//get caractere
			if(lenLine > ((nbAllocMemoryLine * ALLOC_MEMORY) - 2)){
				//Réalloc memory line
				nbAllocMemoryLine++;

				tstr = (char*) realloc(line,
						nbAllocMemoryLine * ALLOC_MEMORY * sizeof(char));
				if(tstr == NULL) {
					printf("erreur\n");
				} else {
					line = tstr;
				}
			}

			if(read(0, &tmp, 1) < 1){
				write(1, "\n", 1);
				if(errno == 0){
					b_exit = 1;
					break;
				} else if(errno == 9){
					dup2(dup_in, STDIN_FILENO);
					errno = 0;
					break;
				} else {
					fprintf(stderr, "errno: %s.\n", strerror(errno));
				}
				fflush(stdout);
				errno = 0; 
				//Reset errno sinon le message d'erreur est bloqué
				//tant qu'il n'y à pas de mise à 0
			}
			line[lenLine++] = *tmp;
			line[lenLine--] = '\0';

			//Parse chaque nouveaux caractere dans la chaine
			//Parse le caractere courant et analyse avec le caractere n-1, n-2
			//(Ctrl-1, alt-1)
			//Si nouveau caractere action -> b_notAction = false;
			
			switch(sh_parseCaractere(line)){
				case 1 :
					//Tab complémentation
					////Penser aussi à la fleche du haut historique
					//sh_execComplémentation();

					break;
				case 2 :
					//action execute
					b_notAction = 0;
					break;
				case 0 :
				default:
					//Nothing
					break;
			}

		}
		
		//execute action
		sh_getArgs(&line, &argArray);

		retExec = (sh_searchFtExec(&argArray))(&argArray);
		//sh_exec(&argArray); //Gérer dabord les builtins
								  //puis le path
		retExec = retExec; //Not Used

		free(argArray);
		argArray = NULL;

	}

	free(line);
	if(argArray != NULL){
		free(argArray);
	}
			
	return (EXIT_SUCCESS);
}

/*EOF*/
