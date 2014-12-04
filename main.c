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
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include "Workshop.h"
#include "Structures.h"

#define NB_SEM 99
#define NbMiddleStep 2
#define NbMutex 1
#define NbCond 1

/* Global variables */
sem_t *semTab[NB_SEM];
pthread_t threadTab[NbMiddleStep];
pthread_mutex_t mutexTab[NbMutex];
pthread_cond_t condTab[NbCond];

/* Function used to post the error messages */
void error(const char* msg) { perror(msg); exit(-1); }

/* Function used as the mask of the signal SIGINT */
void applicateWhenSIGINT(int s) { printf("\nProblem : received stop signal !\n"); exit(-1); }

int main(int argc, char* argv[])
{
	int nbProduitDemande, i, j, k, l;
	pthread_t t1, t2, t3, t4;
	char nb[10];
	char *semName, *txt;

	/* Allocations */
	txt = (char*) malloc(50*sizeof(char));

	/* Initialization : semaphores, conditions, mutex, available pieces */
	for(k = 0; k<NbMutex; k++)
		pthread_mutex_init(&mutexTab[k], NULL);
	for(l = 0; l<NbCond; l++)	
		pthread_cond_init(&condTab[l], NULL);

	for(i = 0; i<NB_SEM; i++)
	{
		/* Semaphore's name */
		sprintf(txt, "sem");
		sprintf(nb, "%d", i);
		semName = strcat(txt, nb);
		semTab[i] = sem_open(semName, O_RDWR | O_CREAT, 0666, 0);
	}

	signal(SIGINT,applicateWhenSIGINT);

	/* Client's choice */
	printf("How many products do you want ?\n");
	scanf("%d", &nbProduitDemande);
	printf("Preparation of the order in progress (%d products) ...\n", nbProduitDemande);

	/*Threads creation workshop*/
	if(pthread_create(&t1, NULL, Launching_board_thread_fct, 0) != 0) { error("Error Launching_board_thread creation\n"); exit(-1);}
	if(pthread_create(&t2, NULL, Postman_thread_fct, 0) != 0) { error("Error Postman_thread creation\n"); exit(-1);}
	if(pthread_create(&t3, NULL, Supplier_Step_thread_fct, 0) != 0) { error("Error Supplier_Step_thread creation\n"); exit(-1);}
	if(pthread_create(&t4, NULL, Final_Product_thread_fct, 0) != 0) { error("Error Final_Product_thread creation\n"); exit(-1);}
	for(j = 0; j <= NbMiddleStep; j++)
	{
		if(pthread_create(&threadTab[i], NULL, Middle_Step_thread_fct, 0) != 0)
			{ error("Error Final_Step_thread creation\n"); exit(-1);}
	}


	/*Delete semaphores */
	for(i = 0; i<NB_SEM; i++)
	{
		/* Semaphore's name */
		sprintf(txt, "sem");
		sprintf(nb, "%d", i);
		semName = strcat(txt, nb);
		sem_unlink(semName);
	}

	free(txt); /* Free the memory allocated to txt with malloc */

	pthread_exit(NULL); /* Destroy the main but not the threads in progress */
}
