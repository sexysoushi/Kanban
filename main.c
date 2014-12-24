/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : December 24, 2014
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
	//if elemSize doesn't increase when nbMiddleStep increase, there are some problems when list_print_Card() is called 
	int elemSize = nbMiddleStep;
	int i;  
	int *tabNumber;
	int cardNumOrder[4] = {0, 1, 2, 3};
	char **cardWorkshopName, **cardRefPiece, **cardDesignationPiece, **cardNameWorkshopSupplier;
	char *elem_cardWorkshopName, *elem_cardRefPiece, *elem_cardDesignationPiece, *elem_cardNameWorkshopSupplier;
	
	/* Initializations */
	cardWorkshopName = (char **) malloc(elemSize * sizeof(char *));
	cardRefPiece = (char **) malloc(elemSize * sizeof(char *));
	cardDesignationPiece = (char **) malloc(elemSize * sizeof(char *));
	cardNameWorkshopSupplier = (char **) malloc(elemSize * sizeof(char *));
	
	for(i=0; i<nbDiffentCard; i++)
	{
		if(i==0)
		{
			elem_cardWorkshopName = concatStringInt("Supplier", i+1);
			elem_cardRefPiece = concatStringInt("S", i+1);
			elem_cardNameWorkshopSupplier = "none";
		}
		else 
		{
			if(i==1)
				elem_cardNameWorkshopSupplier = concatStringInt("Supplier", i); 
			else
				elem_cardNameWorkshopSupplier = concatStringInt("Workshop", i-1); 
				
			elem_cardWorkshopName = concatStringInt("Workshop", i);
			elem_cardRefPiece = concatStringInt("W", i);
		}
		
		elem_cardDesignationPiece = concatStringInt("Part", i+1);
		
		cardWorkshopName[i] = elem_cardWorkshopName;
		cardRefPiece[i] = elem_cardRefPiece;
		cardDesignationPiece[i] = elem_cardDesignationPiece;
		cardNameWorkshopSupplier[i] = elem_cardNameWorkshopSupplier;
	}		
	
	/* Tab for workshop's number */
	tabNumber = (int*) malloc(nbMiddleStep * sizeof(int));
	
	for(i=1; i<nbMiddleStep+1; i++)
		tabNumber[i-1] = i;
	
	/* Mutex */
	for(i=0; i<nbMutex; i++)
		pthread_mutex_init(&mutexTab[i], NULL);
	pthread_mutex_init(&initCardRef, NULL);
	pthread_mutex_init(&stopMutex, NULL);
	
	/* Conditions */
	for(i=0; i<nbCond; i++)	
		pthread_cond_init(&condTab[i], NULL);	

	/* Mask for SIGINT signal */
	signal(SIGINT,applicateWhenSIGINT);

	printf("\n");

	/* Declaration of the reference card for each workshop */
	referenceListCard = list_new();
	initListPossibleCard(cardWorkshopName, cardRefPiece, cardDesignationPiece, cardNameWorkshopSupplier, cardNumOrder);

	list_print_Card(referenceListCard);
	
	workshopList = list_new();
	
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

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	
	for(i=0; i<nbMiddleStep; i++)
		pthread_join(threadTab[i],NULL); 
		
	printf("\n all Join reached \n");
	
	list_delete(&referenceListCard);
	list_delete(&workshopList);
	
	//free(cardWorkshopName);
	//free(cardRefPiece);
	//free(cardDesignationPiece);
	//free(cardNameWorkshopSupplier);
	free(tabNumber);
	
	pthread_mutex_destroy(&initCardRef);
	pthread_mutex_destroy(&stopMutex);
	for(i=0; i<nbMutex; i++)
		pthread_mutex_destroy(&mutexTab[i]);
		
	for(i=0; i<nbCond; i++)
		pthread_cond_destroy(&condTab[i]);

	return 0;
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
