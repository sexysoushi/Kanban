/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : January 2, 2015
 */

#include "Initialize.h"
#include "Structures.h"
#include "Workshop.h"

/* Function used to post the error messages */
void error(const char* msg) { perror(msg); exit(-1); }

/* Function used as the mask of the signal SIGINT */
void applicateWhenSIGINT(int s) 
{	
	Workshop *tmpWorkshop;
	int i;
	
	pthread_cond_signal(&condLBWakeUp);
	pthread_cond_signal(&condPostmanWakeUp);
	
	if(nbProductsWanted > 0)
	{
		tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar("Supplier1", workshopList);
		list_delete(&(tmpWorkshop->stock.listContainer));
		list_delete(&(tmpWorkshop->bal.listCard));
		free(tmpWorkshop);
	
		for(i=1; i<nbMiddleStep+1; i++)
		{
			tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar(concatStringInt("Workshop",i), workshopList);
			list_delete(&(tmpWorkshop->stock.listContainer));
			list_delete(&(tmpWorkshop->bal.listCard));
			free(tmpWorkshop);			
		}
	
		tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar("Final_product1", workshopList);	
		list_delete(&(tmpWorkshop->stock.listContainer));
		list_delete(&(tmpWorkshop->bal.listCard));
		free(tmpWorkshop);
	}
		
	free(tabNumber);
	
	list_delete(&referenceListCard);
	list_delete(&workshopList);
	list_delete(&postmanListCard);

	pthread_mutex_destroy(&mutexThreadCreated);	
	pthread_mutex_destroy(&initCardRef);
	pthread_mutex_destroy(&stopMutex);
	pthread_mutex_destroy(&mutexPostman_listcard);
	pthread_mutex_destroy(&mutexListWorkshop);
	for(i=0; i<nbMutex; i++)
		pthread_mutex_destroy(&mutexTab[i]);

	pthread_cond_destroy(&condPostmanWakeUp);
	pthread_cond_destroy(&condLBWakeUp);
	for(i=0; i<nbCond; i++)
		pthread_cond_destroy(&condTab[i]);

	error("\nProblem : received stop signal !\n"); 
}


int main(int argc, char* argv[])
{
	nbProductsFinished = 0;
	nbThreadCreated = 0;
	//if elemSize doesn't increase when nbMiddleStep increase, there are some problems when list_print_Card() is called 
	int elemSize;
	int i, checkNumber = 0;  
	char term;
	int cardNumOrder[4] = {0, 1, 2, 3};
	char *elem_cardWorkshopName, *elem_cardRefPiece, *elem_cardDesignationPiece, *elem_cardNameWorkshopSupplier;
	
	nbMiddleStep = 0;
	
	if(argc == 2)
	{
		nbMiddleStep = atoi(argv[1]);
		
		if(nbMiddleStep >= 1)
		{
			nbDiffentCard = nbMiddleStep +1;
			nbMutex = nbMiddleStep +2;
			nbCond = nbMiddleStep +2;
			elemSize  = nbMiddleStep;

			threadTab = (pthread_t*) malloc(nbMiddleStep * sizeof(pthread_t));
			condTab = (pthread_cond_t*) malloc(nbCond * sizeof(pthread_cond_t));
			mutexTab = (pthread_mutex_t*) malloc(nbMutex * sizeof(pthread_mutex_t));
		
			nbProductsWanted = -1;
	
			/* Client's choice */
			printf("How many products do you want ? (give an integer)\n");
			if(scanf("%d%c", &nbProductsWanted, &term) != 2 || term != '\n')
				printf("failure not an integer\n");
			else
			{
				checkNumber = 1;
				printf("valid integer !\n");
			}
	
			if(checkNumber == 1)
			{
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
		
				pthread_mutex_init(&mutexThreadCreated, NULL);		
				pthread_mutex_init(&initCardRef, NULL);
				pthread_mutex_init(&stopMutex, NULL);
				pthread_mutex_init(&mutexPostman_listcard, NULL);
				pthread_mutex_init(&mutexListWorkshop, NULL);
	
				/* Conditions */
				for(i=0; i<nbCond; i++)	
					pthread_cond_init(&condTab[i], NULL);	
		
				pthread_cond_init(&condPostmanWakeUp, NULL);
				pthread_cond_init(&condLBWakeUp, NULL);

				/* Mask for SIGINT signal */
				signal(SIGINT,applicateWhenSIGINT);

				printf("\n");

				/* Declaration of the reference card for each workshop */
				referenceListCard = list_new();
				initListPossibleCard(cardWorkshopName, cardRefPiece, cardDesignationPiece, cardNameWorkshopSupplier, cardNumOrder);

				list_print_Card(referenceListCard);
	
				workshopList = list_new();
	
				printf("Preparation of the order in progress (%d products) ...\n", nbProductsWanted);
	
				/* Thread reation for each element */
				if(pthread_create(&t1, NULL, Launching_board_thread_fct, 0)!=0)
					{error("Error Launching_board_thread creation\n"); exit(-1);}
				if(pthread_create(&t2, NULL, Postman_thread_fct, 0) != 0) 
					{ error("Error Postman_thread creation\n"); exit(-1);}
				if(pthread_create(&t3, NULL, Supplier_Step_thread_fct, (void*) &(tabNumber[0])) != 0) 
					{ error("Error Supplier_Step_thread creation\n"); exit(-1);}
				if(pthread_create(&t4, NULL, Final_Product_thread_fct, (void*) &(tabNumber[0])) != 0) 
					{ error("Error Final_Product_thread creation\n"); exit(-1);}
	
				for(i=0; i<nbMiddleStep; i++)
					if(pthread_create(&threadTab[i], NULL, Middle_Step_thread_fct, (void*) &(tabNumber[i])) != 0)
						{ error("Error Final_Step_thread creation\n"); exit(-1);}

				pthread_join(t2,NULL);
				pthread_join(t1,NULL);
				pthread_join(t3,NULL);
				pthread_join(t4,NULL);
	
				for(i=0; i<nbMiddleStep; i++)
					pthread_join(threadTab[i],NULL);
	
				printf("\n all Join reached \n");
	
				list_delete(&referenceListCard);
				list_delete(&workshopList);
		
				free(tabNumber);
			
				pthread_mutex_destroy(&mutexThreadCreated);
				pthread_mutex_destroy(&initCardRef);
				pthread_mutex_destroy(&stopMutex);
				pthread_mutex_destroy(&mutexPostman_listcard);
				pthread_mutex_destroy(&mutexListWorkshop);
				for(i=0; i<nbMutex; i++)
					pthread_mutex_destroy(&mutexTab[i]);
		
				pthread_cond_destroy(&condPostmanWakeUp);
				pthread_cond_destroy(&condLBWakeUp);
				for(i=0; i<nbCond; i++)
					pthread_cond_destroy(&condTab[i]);
			}
		}
		else
			printf("To start : ./main <nbWorkshop> ; nbWorkshop >= 1\n");
	}
	else
		printf("To start : ./main <nbWorkshop>\n");
	
	return 0;
}


