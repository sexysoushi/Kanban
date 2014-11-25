/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : November 22, 2014
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include "Structures.h"
#include "Supplier.h"
#include "Step.h"

/* Function used to post the error messages */
void error(const char* msg) { perror(msg); exit(-1); }

/* Function used as the mask of the signal SIGINT */
void applicateWhenSIGINT(int s) { printf("\nProblem : signal stop\n"); exit(-1); }
 
int main(int argc, char* argv[])
{	
	int nbFinishProducts;
	pthread_t t1, t2,t;
	pStep st_Step;
	
	signal(SIGINT,applicateWhenSIGINT);
	
	/* Allocations */
	st_Step = (pStep) malloc(sizeof(Step));
	
	/* Initialisation : conditions, mutex, available pieces */
	st_Step->nbPieces = 0;
	
	/*Threads creation */
	if(pthread_create(&t1, NULL, Supplier_thread_fct, 0) != 0) { error("Error Supplier thread creation\n"); exit(1);}
	if(pthread_create(&t2, NULL, Step_thread_fct, (void*) &st_Step) != 0) { error("Error Step thread creation\n"); exit(1); }
	
	/* Client's choice */
	printf("How many products do you want ?\n");
	scanf("%d", &nbFinishProducts);	
	printf("Preparation of the order in progress (%d products) ...\n", nbFinishProducts);
	
	sleep(5);
	
	return 0;
}
