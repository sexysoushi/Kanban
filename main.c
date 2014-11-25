/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : November 22, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include "Workshop.h"

#define NB_SEM 99
sem_t semTab[NB_SEM];

/* Function used to post the error messages */
void error(const char* msg) { perror(msg); exit(-1); }

/* Function used as the mask of the signal SIGINT */
void applicateWhenSIGINT(int s) { printf("\nProblem : signal stop\n"); exit(-1); }
 
int main(int argc, char* argv[])
{	
	int nbProduitDemande, i;
	pthread_t t1, t2,t3;
	char nb[50];	
	char *semName, *txt;

	/* Noms semaphores*/
	sprintf(txt, "sem");
	sprintf(nb, "%d", i);
	semName = strcat(txt, nb);
	
	signal(SIGINT,applicateWhenSIGINT);
	
	/* Allocations */
	txt = (char*) malloc(50*sizeof(char));
	
	/* Initialisation : semaphores, conditions, mutex, available pieces 
	for(i = 0; i<NB_SEM; i++)
	{
		semTab[i] = sem_open(semName, O_RDWR|O_CREAT, 0666, 0);
	}
	*/
	/* Client's choice */
	printf("How many products do you want ?\n");
	scanf("%d", &nbProduitDemande);	
	printf("Preparation of the order in progress (%d products) ...\n", nbProduitDemande);

	/*Threads creation workshop*/
	//if(pthread_create(&t1, NULL, Workshop_thread_fct, 0) != 0) { error("Error Workshop thread creation\n"); exit(1);}
	//if(pthread_create(&t2, NULL, Workshop_thread_fct, 0) != 0) { error("Error Workshop thread creation\n"); exit(1);}
	//if(pthread_create(&t3, NULL, Workshop_thread_fct, 0) != 0) { error("Error Workshop thread creation\n"); exit(1);}
	
	sleep(5); //?

	/*Suppression semaphores
	for(i = 0; i<NB_SEM; i++)
	{
		semTab[i] = sem_unlink(semName);
	}
*/
	pthread_exit(NULL); // le main se détruit mais pas les threads encore en cours
}
