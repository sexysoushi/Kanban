/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#include "Workshop.h"

/*
Un homme flux récupère régulièrement les cartes aux différents postes de montage et les apporte à l’atelier en amont.
*/
void* Postman_thread_fct(void* arg)
{
	int returnSignal;
	int i,j;
	postmanListCard = list_new();
	Workshop* tmpWorkshop;
	list* tmpListCard;
	Card* tmpCard;
	
	// wait initialisations ends
	sleep(3);
	
	while(1)
	{		  
		pthread_mutex_lock(&mutexPostman_listcard); 

		//on recupère les cartes magnetiques de chaque BAL et on les mets dans la liste du postman
		pthread_mutex_lock(&mutexListWorkshop); 
		
		list_first(workshopList);
		for(i=0; i<workshopList->nbElem; i++)
		{
			tmpWorkshop = (Workshop*) list_data(workshopList);
			tmpListCard = tmpWorkshop->bal.listCard;
			
			if(tmpListCard && tmpListCard->nbElem != 0)
			{
				list_first(tmpListCard);
								
				for(j=0; j<tmpListCard->nbElem; j++)
				{
					tmpCard = (Card*) list_data(tmpListCard);
					list_insert(postmanListCard, tmpCard);
					list_removeFirst(tmpListCard);
				}
			}
			list_next(workshopList);
		}
		pthread_mutex_unlock(&mutexListWorkshop);	 

		// Wake up the LB for cards transmission
		do
		{
			returnSignal = pthread_cond_signal(&condLBWakeUp);
		}while(returnSignal != 0);

		// Wait for LB signal
		pthread_cond_wait(&condPostmanWakeUp, &mutexPostman_listcard);
		
		pthread_mutex_unlock(&mutexPostman_listcard); 

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished == nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
		
		pthread_mutex_unlock(&stopMutex);
	}
	printf("<=== sortie Postman\n");
	
	list_delete(&postmanListCard);
}


/*
Tableau de lancement reveil poste en amont afin qu'il refournisse en pieces la carte magnetique détient la référence
*/
void* Launching_board_thread_fct(void* arg)
{
	int returnSignal;
	int i,j;
	Card *tmpCard;
	char* tmpChar;
	
	while(1)
	{
		pthread_mutex_lock(&mutexPostman_listcard); 	

		// Wait for Postman signal
		pthread_cond_wait(&condLBWakeUp, &mutexPostman_listcard);

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished < nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);

			list_first(postmanListCard);
		
			for(i=0; i<postmanListCard->nbElem; i++)
			{
				tmpCard = (Card*) list_data(postmanListCard);
			
				for(j=1; j<(nbMiddleStep+1); j++)
				{	
					if((strcmp(tmpCard->workshopName,concatStringInt("Supplier", j))) == 0)
					{
						// here with just one supplier j-1 must be equal to 0
						pthread_mutex_lock(&mutexTab[j-1]);
						do
						{
							returnSignal = pthread_cond_signal(&condTab[j-1]);
						}while(returnSignal != 0);
						pthread_mutex_unlock(&mutexTab[j-1]);
						break;
					}
								
					if((strcmp(tmpCard->workshopName,concatStringInt("Workshop", j))) == 0)
					{
						pthread_mutex_lock(&mutexTab[j]);
						do
						{
							returnSignal = pthread_cond_signal(&condTab[j]);
						}while(returnSignal != 0);
						pthread_mutex_unlock(&mutexTab[j]);
						break;
					}
				}
					
				list_removeFirst(postmanListCard);
				list_next(postmanListCard);
			}
			pthread_mutex_unlock(&mutexPostman_listcard);
			
			// Wake up the LB for receiving all cards
			do
			{
				returnSignal = pthread_cond_signal(&condPostmanWakeUp);
			}while(returnSignal != 0);
		}
		else
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
	}
	printf("<=== sortie Launching_board\n");
	
	// Frees and exit
	pthread_exit(NULL);
}


void* Supplier_Step_thread_fct(void* arg)
{ 	
	Container tmpContainer = initContainerWorkshop();
	Workshop *tmpWorkshop;
	int* numberThread = (int*) arg;
	Workshop *supplier = (Workshop*) malloc(sizeof(Workshop));

	supplier = initWorkshop(supplier, "Supplier", *numberThread, -1);	
	list_insert(workshopList, supplier);
		
	while(1)
	{
		pthread_mutex_lock(&mutexTab[0]);
		
		//reveil du workshop par le tableau de lancement
		pthread_cond_wait(&condTab[0], &mutexTab[0]);
		
		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished < nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
		
			printf("%s : Container's creation in progress...\n", supplier->name); 
			sleep(minusFabricationTime);
			printf("%s : Container created !\n", supplier->name);
		
			//met la piece fabriquée dans un container destiné au poste en aval
			supplier->containerToSend->nbPieces += nbPieceByContainer;

			//envoi le container dans le stock du post en aval
			if(supplier->containerToSend->nbPieces == nbPieceByContainer)
			{
				tmpWorkshop = list_seekWorkshopName_voidstar("Workshop1", workshopList);
				tmpContainer = *supplier->containerToSend;
				list_insert(tmpWorkshop->stock.listContainer, (void*) &tmpContainer);

				printf("%s : Container envoye !\n", supplier->name);		
				supplier->containerToSend->nbPieces = 0;
			}
		
			pthread_mutex_unlock(&mutexTab[0]);
		}
		else
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
	}
	printf("<=== sortie %d\n", 0);

	
	// Frees and exit
	list_delete(&(supplier->stock.listContainer));
	list_delete(&(supplier->bal.listCard));
	free(supplier);
	
	pthread_exit(NULL);
}


void* Middle_Step_thread_fct(void* arg)
{
	int returnSignal;
	Workshop *tmpWorkshop;
	int* numberThread = (int*) arg;
	Workshop *workshop = (Workshop*) malloc(sizeof(Workshop));
	Container tmpContainer = initContainerWorkshop();
		
	workshop = initWorkshop(workshop, "Workshop", *numberThread, 0);	
	list_insert(workshopList, workshop);
	
	while(1)
	{			
		pthread_mutex_lock(&mutexTab[*numberThread]);
		
		//reveil du workshop par le tableau de lancement
		pthread_cond_wait(&condTab[*numberThread], &mutexTab[*numberThread]);

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished < nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);

			//prend un container du stock si actualcontainer = 0;	
			if(workshop->actualUsedContainer.nbPieces == 0)
			{	
				workshop = takeNewContainerFromStock(workshop);
				printf("%s : Nouveau container pris \n", workshop->name);
			}
			
			//consomme le container
			workshop->actualUsedContainer.nbPieces -= nbPieceByContainer;

			//si une piece a ete prise dans le container
			if(workshop->actualUsedContainer.nbPieces != nbPieceByContainer)
			{
				// Mettre carte dans la boite aux lettres
				list_insert(workshop->bal.listCard, (void*) workshop->actualUsedContainer.magneticCard);
				printf("%s : Carte dans BAL \n", workshop->name);
			}
		
			printf("%s : Container's creation in progress...\n", workshop->name); 
			sleep(minusFabricationTime);
			printf("%s : Container created !\n", workshop->name);
		
			//met la piece fabriquée dans un container destiné au poste en aval		
			workshop->containerToSend->nbPieces += nbPieceByContainer;
			
			//envoi le container dans le stock du post en aval
			if(workshop->containerToSend->nbPieces == nbPieceByContainer)
			{
				//if this is not the last Middle_step
				if((*numberThread+1) != (nbMiddleStep+1))
					tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar(concatStringInt("Workshop",*numberThread+1), workshopList);
				else
					tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar("Final_product1", workshopList);

				tmpContainer = *workshop->containerToSend;
				list_insert(tmpWorkshop->stock.listContainer, (void*) &tmpContainer);
								
				printf("%s : Container envoye \n", workshop->name);	
				
				if((*numberThread+1) == (nbMiddleStep+1))
				{
					pthread_mutex_lock(&mutexTab[nbMiddleStep+1]);
					do
					{
						returnSignal = pthread_cond_signal(&condTab[nbMiddleStep+1]);
					}while(returnSignal != 0);
					pthread_mutex_unlock(&mutexTab[nbMiddleStep+1]);
				}
						
				workshop->containerToSend->nbPieces = 0;
			}

			//Envoi du container vide au poste en amont !
			//if(workshop->actualUsedContainer.nbPieces == 0)
				//list_insert(workshop->actualUsedContainer, workshopEnAmont->stock.listContainer);
		
			pthread_mutex_unlock(&mutexTab[*numberThread]);
		}
		else
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
	}
	printf("<=== sortie %d\n", *numberThread);

	// Frees and exit
	list_delete(&(workshop->stock.listContainer));
	list_delete(&(workshop->bal.listCard));
	free(workshop);
		
	pthread_exit(NULL);
}


void* Final_Product_thread_fct(void* arg)
{
	int i, returnSignal;
	int* numberThread = (int*) arg;
	
	Workshop *finalProduct = (Workshop*) malloc(sizeof(Workshop));
	
	finalProduct = initWorkshop(finalProduct, "Final_product", *numberThread, 1);	
	list_insert(workshopList, finalProduct);
	
	while(1)
	{
		printf("111111\n");
		// si le stock n'est pas vide ou si le conteneur en cours n'est pas fini ==> on créé une autre piece
		pthread_mutex_lock(&mutexTab[nbMiddleStep+1]);
		printf("222222\n");
		//if((finalProduct->stock.nbContainer !=0) || (finalProduct->actualUsedContainer.nbPieces > 0))
		//{
		//prend un container du stock si actualcontainer = 0;	
		if(finalProduct->actualUsedContainer.nbPieces == 0)
		{	
			if(nbProductsFinished > 0)
			{
				pthread_cond_wait(&condTab[nbMiddleStep+1], &mutexTab[nbMiddleStep+1]);
				printf("toto\n");
			}

			finalProduct = takeNewContainerFromStock(finalProduct);
			printf("%s : Nouveau container pris \n", finalProduct->name);
		}
		pthread_mutex_unlock(&mutexTab[nbMiddleStep+1]);
		
		// ici prendre une piece dans le container
		finalProduct->actualUsedContainer.nbPieces--;

		//si prise de la premiere piece du conteneur
		if(finalProduct->actualUsedContainer.nbPieces != nbPieceByContainer)
		{
			// Mettre carte dans la boite aux lettres 
			list_insert(finalProduct->bal.listCard, (void*) finalProduct->actualUsedContainer.magneticCard);
			printf("%s : Carte dans BAL \n", finalProduct->name);
		}

		//fabrication de la piece
		printf("%s : Piece's creation in progress...\n", finalProduct->name); 
		sleep(minusFabricationTime+1);
		printf("%s : 1 Piece finished !\n", finalProduct->name);
		
		//Envoi du container vide au poste en amont !
		//if(finalProduct->actualUsedContainer.nbPieces == 0)
			//list_insert(finalProduct->actualUsedContainer, workshopEnAmont->stock.

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		nbProductsFinished ++;
	
		printf("Number of finished products : %d\n", nbProductsFinished);
	
		if(nbProductsFinished == nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
		pthread_mutex_unlock(&stopMutex);
		//}
		//pthread_mutex_unlock(&mutexTab[nbMiddleStep+1]);
	}
	printf("<=== sortie %d\n", nbMiddleStep+1);
	printf("Shutdown the program, please wait ...\n");
	
	//while(finalProduct->stock.nbContainer != 2);
	
	// wake up all for the end of the program
	for(i=0; i<nbCond+1; i++)
	{
		printf("Fin\n");
		do
		{
			returnSignal = pthread_cond_signal(&condTab[i]);
		}while(returnSignal != 0);
	}
	
	// Frees and exit
	list_delete(&(finalProduct->stock.listContainer));
	list_delete(&(finalProduct->bal.listCard));
	free(finalProduct);
	
	pthread_exit(NULL);
}

Workshop* takeNewContainerFromStock(Workshop* w)
{
	Container *tmpContainer;

	list_first(w->stock.listContainer);
	if(w->stock.listContainer->nbElem > 0)
	{
		tmpContainer = (Container*) list_data(w->stock.listContainer);
		w->actualUsedContainer = *tmpContainer;
		list_removeFirst(w->stock.listContainer);
		w->stock.nbContainer--;
	}
	else
		printf("%s : Can't take new container because of an empty stock\n", w->name);
		
	return w;
}





