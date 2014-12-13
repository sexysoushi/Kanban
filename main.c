/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : November 22, 2014
 */

#include "Initialize.h"
#include "Structures.h"
#include "Workshop.h"


/* Function used to post the error messages */
void error(const char* msg) { perror(msg); exit(-1); }

/* Function used as the mask of the signal SIGINT */
void applicateWhenSIGINT(int s) 
{
	error("\nProblem : received stop signal !\n"); 
}


int main(int argc, char* argv[])
{
	int i;
	int *tabNumber;
	
	/* Initializations */
	/* Tab for workshop's number */
	tabNumber = (int*) malloc(nbMiddleStep * sizeof(int));
	for(i=1; i<nbMiddleStep+1; i++)
		tabNumber[i-1] = i;
	
	/* Mutex */
	for(i=0; i<nbMutex; i++)
		pthread_mutex_init(&mutexTab[i], NULL);
	
	/* Conditions */
	for(i=0; i<nbCond; i++)	
		pthread_cond_init(&condTab[i], NULL);	

	/* Mask for SIGINT signal */
	signal(SIGINT,applicateWhenSIGINT);

	/* Client's choice */
	printf("How many products do you want ?\n");
	scanf("%d", &nbProductsWanted);
	printf("Preparation of the order in progress (%d products) ...\n", nbProductsWanted);

	/*Threads creation workshop*/
	if(pthread_create(&t1, NULL, Launching_board_thread_fct, 0)!=0)
		{error("Error Launching_board_thread creation\n"); exit(-1);}
	if(pthread_create(&t2, NULL, Postman_thread_fct, 0) != 0) 
		{ error("Error Postman_thread creation\n"); exit(-1);}
	if(pthread_create(&t3, NULL, Supplier_Step_thread_fct, (void*) &(tabNumber[0])) != 0) 
		{ error("Error Supplier_Step_thread creation\n"); exit(-1);}
	if(pthread_create(&t4, NULL, Final_Product_thread_fct, (void*) &(tabNumber[0])) != 0) 
		{ error("Error Final_Product_thread creation\n"); exit(-1);}
	
	for(i=0; i<nbMiddleStep; i++)
	{
		if(pthread_create(&threadTab[i], NULL, Middle_Step_thread_fct, (void*) &(tabNumber[i])) != 0)
			{ error("Error Final_Step_thread creation\n"); exit(-1);}
	}

	/* Declaration of the reference card for each workshop */
	referenceListCard = initListPossibleCard();

	pthread_exit(NULL); /* Destroy the main but not the threads in progress */
}



/*for(i=0; i<nbSem; i++)
	{
		//Semaphore's name 
		sprintf(txt, "sem");
		sprintf(nbName, "%d", i);
		semName = strcat(txt, nbName);
		semTab[i] = sem_open(semName, O_RDWR | O_CREAT, 0666, 0);
	}*/
	
	
	
	
	
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
