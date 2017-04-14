/**
 * \file fonc.c
 * \brief fonction minish
 * \author pierre
 * \date 28/03/2017
 * \licence
 *
 */

#include "fonc.h"
#include "builtin.c"


/* -----------------*/
/**
 * \brief Tableau Globale Built-in
 */
/* -----------------*/
//Nom des fonction Built-in
char t_strBuiltin[2][100] = {	{"hello"}, 
								{"cd"}};
//Déclaration du tableau de fonction builtin
//En concordance avec le tableau ci-dessus
int (* t_ftBuiltin[2])(char***) = {bi_hello, bi_cd};

/**
 * \brief Crée un processus en dupplicant le processus appelant
 * 
 * \return pid_t Le PID du processus fils créer
 *
 * La fonction create_process duplique le processus appelant et retourne
 * le PID du processus fils ainsi créé 
 */
pid_t create_process(void)
{
    pid_t pid; //New pid

    do {
		pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));

    return pid;
}

/**
 *
 * \brief fonction appelée par le signal SIGINT
 * 		  ferme le flux stdin pour stoppé read dans le main.
 *
 */
void sh_quit(){
	close(STDIN_FILENO);
}


/* -----------------*/
/**
 * \brief Affichage du prompt
 */
/* -----------------*/
void sh_prompt(void){
	write(1, "$> ", 3);	
}

/* -----------------*/
/**
 * \brief recupere la commande et argument
 *
 * \param str Pointeur de pointeur à passer en paramètre
 *
 * \return 0 ok
 */
/* -----------------*/
int sh_getLine(char **str){
	
	char tmp = ' ';
	int i = 0;
	int j = 1; //initialise la multiplication
	
	char *tstr = NULL;
	
	*str = (char*) calloc(ALLOC_MEMORY, sizeof(char));

	while(tmp != '\n'){
		 
		 (*str)[i++] = tmp; //Espace nécessaire dans l'analyse des arguments
		 tmp = getc(stdin);
		 
		 if( (i+1) >= (j*ALLOC_MEMORY)) {
			 j++;
			 tstr = (char*) realloc(*str, j*ALLOC_MEMORY * sizeof(char));
			 if(tstr == NULL) {
				 perror("Err : realloc getLine");
			 } else {
				 *str = tstr;
			 }
		 }
	}
	
	(*str)[i] = '\0';
	
	return 0; //Ok
}

/* -----------------*/
/**
 * \brief Parse le dernier caractere de la ligne pour analyser caractère action
 *
 * \param line Ligne de commande utilisateur
 *
 * \return 0 default pas d'action
 * 		   1 Tab complémentation
 * 		   2 Execute action
 */
/* -----------------*/
int sh_parseCaractere(char *line)
{
	int strLen = strlen(line);
	strLen--;
	//printf("car: %d", line[strLen]);

	switch(line[strLen])
	{
		case '\n' :
			line[strLen] = '\0'; //retenu du caractere
			return (2); //caractere action
			break;
		default : //Return 0
			break;
	}

	return (0);
}

/* -----------------*/
/**
 * \brief Parse le paramètre in et récupère la commande et ces arguments
 *
 * \param in Ligne de commande entrée après le prompt
 * \param out Tableau de string comprenant la commmande et les arguments
 *
 * \return 0 OK
 */
/* -----------------*/
int sh_getArgs(char **in, char ***out){
	
	//allocate memory out
	*out = (char**) calloc(256, sizeof(char*));
	
	int i=0;
	
	if(*out == NULL){
		exit(EXIT_FAILURE);
	}
	
	char *pch = NULL;
	
	pch = strtok (*in," ");
	while (pch != NULL)
	{
		(*out)[i++] = pch;
		pch = strtok (NULL, " ");
	}
	
	//termine array arguments
	(*out)[i] = NULL;
		
	return 0; //Ok
}

/* -----------------*/
/**
 * \brief Fonction d'éxecution de la commande et de ces arguments
 *
 * \param args Tableau de string comprenant la commmande et les arguments
 * 		  pid  Pointeur de type pid_t contenant le pid précédent
 *
 * \return 0 OK 
 */
/* -----------------*/
int sh_exec(char ***args){

	int status = 0;
	static pid_t pid;

	if((*args)[0] == NULL){
		return -1; //Code de retour pas de commande
		
	} else {
		
		pid = create_process();

		switch (pid) {
		case -1:
			//Erreur
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		
		case 0:
			//Processus Fils
			if(execvp((*args)[0], *args) == -1){
				fprintf(stderr, "%s : command not found\n", (*args)[0]);
				kill(getpid(), SIGTERM);
			}
			break;
		
		default:
			//Processus Père
			if(wait(&status) == -1){
				 perror("wait");
				 exit(EXIT_FAILURE);
			}
			break;
		}
	}
	
	return 0; //Ok
}

/* -----------------*/
/**
 * \brief Fonction qui retourne une fonction à éxécuter correspndant à la commande entré.
 *
 * \param args Tableau de string comprenant la commmande et les arguments
 *
 * \return fonction d'éxécution de la commande Builtin
 * \return fonction sh_exec qui recherche dans le path la commande
 */
/* -----------------*/
int (* sh_searchFtExec(char ***args))(char***){

	//Recherche la fonction builtin dans args 
	//retourne la fonction correspondante
	//sinon renvoie sh_exec path
	
	if((*args)[0] == NULL){
		return sh_exec;
	}
	
	for(int i = 0; i < 2; i++){
		if(strcmp(**args, t_strBuiltin[i]) == 0){
			return t_ftBuiltin[i];
		} 
	}
	return sh_exec;
}

