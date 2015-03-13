/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#include "Workshop.h"

/*
Un homme flux récupère régulièrement les cartes aux différents postes de montage et les apporte à l’atelier en amont.
*/
void* Postman_thread_fct()
{
	int returnSignal;
	int i, initOk = 0;
	postmanListCard = list_new();
	Workshop* tmpWorkshop;
	list* tmpListCard;
	Card* tmpCard;
	
	// wait initialisations ends
	while(initOk == 0)
	{
		pthread_mutex_lock(&mutexThreadCreated);
		if(nbThreadCreated >= nbMiddleStep)
			initOk = 1;
		pthread_mutex_unlock(&mutexThreadCreated);
	}

	sleep(1);
	
	while(1)
	{		  
		//on recupère les cartes magnetiques de chaque BAL et on les mets dans la liste du postman
		pthread_mutex_lock(&mutexListWorkshop); 
		
		list_first(workshopList);
		
		pthread_mutex_lock(&mutexPostman_listcard); 
		for(i=0; i<workshopList->nbElem; i++)
		{
			tmpWorkshop = (Workshop*) list_data(workshopList);
			tmpListCard = tmpWorkshop->bal.listCard;
			
			if(tmpListCard && (tmpListCard->nbElem != 0))
			{
				list_first(tmpListCard);
								
				while(tmpListCard->nbElem > 0)
				{
					tmpCard = (Card*) list_data(tmpListCard);
					list_insert(postmanListCard, tmpCard);
											 
					printf("\t\t\t\t\t\tPostMan : Send card -> %s to LB\n", tmpCard->workshopName);
					list_removeFirst(tmpListCard);
				}
			}
			list_next(workshopList);
		}
		pthread_mutex_unlock(&mutexListWorkshop);
		
		if(postmanListCard->nbElem > 0)
		{
			// Wake up the LB for cards transmission
			do
			{
				printf("\t\t\t\t\t\tPostMan : Send signal to LB\n");
				returnSignal = pthread_cond_signal(&condLBWakeUp);
			}while(returnSignal != 0);
		
			printf("\t\t\t\t\t\tPostMan : Wait for LB wake up signal\n");
			// Wait for LB signal
			pthread_cond_wait(&condPostmanWakeUp, &mutexPostman_listcard);
			printf("\t\t\t\t\t\tPostMan : LB wake up signal received\n");
		}
		else
			sleep(1);
			
		pthread_mutex_unlock(&mutexPostman_listcard); 

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished == nbProductsWanted)
		{
			do
			{
				returnSignal = pthread_cond_signal(&condLBWakeUp);
			}while(returnSignal != 0);
			pthread_mutex_unlock(&stopMutex);
			break;
		}
		
		pthread_mutex_unlock(&stopMutex);
	}
	printf("<=== end Postman\n");
	
	pthread_cond_signal(&condLBWakeUp);
	
	list_delete(&postmanListCard);
	pthread_exit(NULL);
}


/*
Tableau de lancement reveil poste en amont afin qu'il refournisse en pieces la carte magnetique détient la référence
*/
void* Launching_board_thread_fct()
{
	int returnSignal;
	int i,j;
	Card *tmpCard;
	char* tmpChar;
	

	while(1)
	{
		pthread_mutex_lock(&mutexPostman_listcard); 	

		printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : Wait for Postman wake up signal\n");
		// Wait for Postman signal
		pthread_cond_wait(&condLBWakeUp, &mutexPostman_listcard);
		printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : Postman wake up signal received\n");
		
		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished < nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
	
			if(postmanListCard->nbElem > 0)
			{	
				printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : nb card to take care of -> %d\n", postmanListCard->nbElem);
	
				for(i=0; i<postmanListCard->nbElem; i++)
				{
					list_first(postmanListCard);
					tmpCard = (Card*) list_data(postmanListCard);
					printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : Card for %s received\n", tmpCard->workshopName);
		
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
							printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : Send signal to %s\n", tmpCard->workshopName);
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
							printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : Send signal to %s\n", tmpCard->workshopName);
							pthread_mutex_unlock(&mutexTab[j]);
							break;
						}
					}
				
					list_removeFirst(postmanListCard);
					//list_next(postmanListCard);
				}
			}
			// Wake up the LB for receiving all cards
			do
			{
				printf("\t\t\t\t\t\t\t\t\t\t\t\tLB : Send signal to PostMan\n");
				returnSignal = pthread_cond_signal(&condPostmanWakeUp);
			}while(returnSignal != 0);
			
			pthread_mutex_unlock(&mutexPostman_listcard);
		}
		else
		{
			do
			{
				returnSignal = pthread_cond_signal(&condPostmanWakeUp);
			}while(returnSignal != 0);
			pthread_mutex_unlock(&stopMutex);
			break;
		}
	}
<<<<<<< Updated upstream
	printf("<=== sortie Launching_board\n");

	pthread_cond_signal(&condPostmanWakeUp);
	
	// Frees and exit
=======
	*/
>>>>>>> Stashed changes
	pthread_exit(NULL);
}


void* Supplier_Step_thread_fct(void* arg)
{ 	
	Container tmpContainer = initContainerWorkshop();
	Workshop *tmpWorkshop, *test;
	int* numberThread = (int*) arg;
	Workshop *supplier = (Workshop*) malloc(sizeof(Workshop));

	supplier = initWorkshop(supplier, "Supplier", *numberThread, -1);	
	list_insert(workshopList, supplier);
		
	while(1)
	{
		pthread_mutex_lock(&mutexTab[0]);
		
		if(nbProductsFinished < nbProductsWanted)
		{
			//reveil du workshop par le tableau de lancement
			pthread_cond_wait(&condTab[0], &mutexTab[0]);
		
			// Protection by mutex because of nbProductsFinished multiple access 
			pthread_mutex_lock(&stopMutex);
		
			pthread_mutex_unlock(&stopMutex);
		
			printf("%s : Container's creation in progress...\n", supplier->name); 
			sleep(minusFabricationTime);
			printf("%s : Container created !\n", supplier->name);
		
			//met la piece fabriquée dans un container destiné au poste en aval
			supplier->containerToSend->nbPieces += nbPieceByContainer;

			// Send the container to the next workshop's stock (2nd condition used when the program is stopping)
			if((supplier->containerToSend->nbPieces == nbPieceByContainer) && (nbProductsFinished < nbProductsWanted))
			{
				tmpWorkshop = list_seekWorkshopName_voidstar("Workshop1", workshopList);
				tmpContainer = *supplier->containerToSend;
				list_insert(tmpWorkshop->stock.listContainer, (void*) &tmpContainer);
				tmpWorkshop->stock.nbContainer ++;
				//printf("%s : Container sent !\n", supplier->name);	
				
				test = list_seekWorkshopName_voidstar("Workshop1", workshopList);
					
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
	
	pthread_mutex_lock(&mutexThreadCreated);
	nbThreadCreated ++;
	pthread_mutex_unlock(&mutexThreadCreated);
	
	while(1)
	{			
		pthread_mutex_lock(&mutexTab[*numberThread]);
		
		if(nbProductsFinished < nbProductsWanted)
		{
			// Wait for the Launching_board wake up signal
			pthread_cond_wait(&condTab[*numberThread], &mutexTab[*numberThread]);

			// Protection by mutex because of nbProductsFinished multiple access 
			pthread_mutex_lock(&stopMutex);
		
			pthread_mutex_unlock(&stopMutex);

			// Take a container from the stock if actualcontainer = 0;	
			if(workshop->actualUsedContainer.nbPieces == 0)
			{	
				workshop = takeNewContainerFromStock(workshop);
				printf("%s : New container taken \n", workshop->name);
			}
			
			// Use pieces inside the container
			workshop->actualUsedContainer.nbPieces -= nbPieceByContainer;

			// if the first piece was taken
			if(workshop->actualUsedContainer.nbPieces != nbPieceByContainer)
			{
				// Put magnetic Card in the mailBox
				list_insert(workshop->bal.listCard, (void*) workshop->actualUsedContainer.magneticCard);
				printf("%s : Card in BAL \n", workshop->name);
			}
		
			printf("%s : Container's creation in progress...\n", workshop->name); 
			sleep(minusFabricationTime);
		
			// Put the new piece in the container which will be sent to the next workshop		
			workshop->containerToSend->nbPieces += nbPieceByContainer;
			
			// Send the container to the next workshop's stock (2nd condition used when the program is stopping)
			if((workshop->containerToSend->nbPieces == nbPieceByContainer) && (nbProductsFinished < nbProductsWanted))
			{
				//if this is not the last Middle_step
				if((*numberThread+1) != (nbMiddleStep+1))
					tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar(concatStringInt("Workshop",*numberThread+1), workshopList);
				// else, send to Final_product workshop
				else
					tmpWorkshop = (Workshop*) list_seekWorkshopName_voidstar("Final_product1", workshopList);

				tmpContainer = *workshop->containerToSend;
				list_insert(tmpWorkshop->stock.listContainer, (void*) &tmpContainer);
				tmpWorkshop->stock.nbContainer ++;		
				//printf("%s : Container sent \n", workshop->name);	
										
				workshop->containerToSend->nbPieces = 0;
			}	
			pthread_mutex_unlock(&mutexTab[*numberThread]);
		}
		else
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
	}

	// Frees and exit
	list_delete(&(workshop->stock.listContainer));
	list_delete(&(workshop->bal.listCard));
	free(workshop);
		
	printf("<=== sortie %d\n", *numberThread);
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
		// si le stock n'est pas vide ou si le conteneur en cours n'est pas fini ==> on créé une autre piece
		pthread_mutex_lock(&mutexTab[nbMiddleStep+1]);
		
		if((finalProduct->stock.nbContainer !=0) || (finalProduct->actualUsedContainer.nbPieces > 0))
		{
			//prend un container du stock si actualcontainer = 0;	
			if(finalProduct->actualUsedContainer.nbPieces == 0)
			{	
				finalProduct = takeNewContainerFromStock(finalProduct);
				printf("%s : New container taken \n", finalProduct->name);
			}
			pthread_mutex_unlock(&mutexTab[nbMiddleStep+1]);
		
			// ici prendre une piece dans le container
			finalProduct->actualUsedContainer.nbPieces--;

			//si prise de la premiere piece du conteneur
			if(finalProduct->actualUsedContainer.nbPieces == nbPieceByContainer-1)
			{
				// Mettre carte dans la boite aux lettres 
				list_insert(finalProduct->bal.listCard, (void*) finalProduct->actualUsedContainer.magneticCard);
				printf("%s : Card in BAL \n", finalProduct->name);
			}

			//fabrication de la piece
			printf("%s : Piece's creation in progress...\n", finalProduct->name); 
			sleep(minusFabricationTime+3);
			printf("%s : 1 Piece finished !\n", finalProduct->name);
		
			//Envoi du container vide au poste en amont !
			//if(finalProduct->actualUsedContainer.nbPieces == 0)
				//list_insert(finalProduct->actualUsedContainer, workshopEnAmont->stock.

			// Protection by mutex because of nbProductsFinished multiple access 
			pthread_mutex_lock(&stopMutex);
			nbProductsFinished ++;
	
			printf("NUMBER OF FINISHED PRODUCTS : %d\n", nbProductsFinished);
	
			if(nbProductsFinished == nbProductsWanted)
			{
				pthread_mutex_unlock(&stopMutex);
				break;
			}
			pthread_mutex_unlock(&stopMutex);
		}
		pthread_mutex_unlock(&mutexTab[nbMiddleStep+1]);
	}
	printf("<=== sortie %d\n", nbMiddleStep+1);
	printf("Shutdown the program, please wait ...\n");
	
	// wake up all for the end of the program
	for(i=0; i<nbCond; i++)
	{
		do
		{
			returnSignal = pthread_cond_signal(&condTab[i]);
		}while(returnSignal != 0);
	}
	
	pthread_cond_signal(&condLBWakeUp);
	pthread_cond_signal(&condPostmanWakeUp);
	
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





