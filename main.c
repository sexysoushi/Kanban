/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : November 22, 2014
 */

#include "Structures.h"
#include "Workshop.h"


/* Function used to post the error messages */
void error(const char* msg) { perror(msg); exit(-1); }

/* Function used as the mask of the signal SIGINT */
void applicateWhenSIGINT(int s) { printf("\nProblem : received stop signal !\n"); exit(-1); }

int main(int argc, char* argv[])
{
	int i;
	pthread_t t1, t2, t3, t4;
	//char nbName[10];
	//char *semName, *txt;

	/* Allocations */
	//txt = (char*) malloc(50*sizeof(char));

	/* Initialization : semaphores, conditions, mutex, available pieces */
	for(i=0; i<nbMutex; i++)
		pthread_mutex_init(&mutexTab[i], NULL);
		
	for(i=0; i<nbCond; i++)	
		pthread_cond_init(&condTab[i], NULL);

	/*for(i=0; i<nbSem; i++)
	{
		//Semaphore's name 
		sprintf(txt, "sem");
		sprintf(nbName, "%d", i);
		semName = strcat(txt, nbName);
		semTab[i] = sem_open(semName, O_RDWR | O_CREAT, 0666, 0);
	}*/

	/* Mask for SIGINT signal */
	signal(SIGINT,applicateWhenSIGINT);

	/* Client's choice */
	printf("How many products do you want ?\n");
	scanf("%d", &nbProductsWanted);
	printf("Preparation of the order in progress (%d products) ...\n", nbProductsWanted);

	/*Threads creation workshop*/
	if(pthread_create(&t1, NULL, Launching_board_thread_fct, 0)!=0){error("Error Launching_board_thread creation\n"); exit(-1);}
	if(pthread_create(&t2, NULL, Postman_thread_fct, 0) != 0) { error("Error Postman_thread creation\n"); exit(-1);}
	if(pthread_create(&t3, NULL, Supplier_Step_thread_fct, 0) != 0) { error("Error Supplier_Step_thread creation\n"); exit(-1);}
	if(pthread_create(&t4, NULL, Final_Product_thread_fct, 0) != 0) { error("Error Final_Product_thread creation\n"); exit(-1);}
	
	for(i=0; i<=nbMiddleStep; i++)
	{
		if(pthread_create(&threadTab[i], NULL, Middle_Step_thread_fct, (void*) &i) != 0)
			{ error("Error Final_Step_thread creation\n"); exit(-1);}
	}


	/*Delete semaphores 
	for(i=0; i<nbSem; i++)
	{
		// Semaphore's name 
		sprintf(txt, "sem");
		sprintf(nbName, "%d", i);
		semName = strcat(txt, nbName);
		sem_unlink(semName);
	}*/

	//free(txt); /* Free the memory allocated to txt with malloc */

	pthread_exit(NULL); /* Destroy the main but not the threads in progress */
}
