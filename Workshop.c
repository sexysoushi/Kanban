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
	int i,j;
	postmanListCard = list_new();
	Workshop* tmpWorkshop;
	list* tmpListCard;
	Card* tmpCard;
	
	printf("===> entree stop Postman\n");
	while(1)
	{		  
		pthread_mutex_lock(&mutexPostman_listcard); 	 
		sleep(1);		
		//on recupère les cartes magnetiques de chaque BAL et les mets dans la liste du postman
		
		pthread_mutex_lock(&mutexListWorkshop); 
		list_first(workshopList);
		for(i=0; i<workshopList->nbElem; i++)
		{
		// Partie à vérifier plus tard 
		// *************************************************************************
			tmpWorkshop = (Workshop*) list_data(workshopList);
			tmpListCard = tmpWorkshop->bal.listCard;
			
			if(tmpListCard && tmpListCard->nbElem != 0)
			{
				list_first(tmpListCard);
				for(j=0; j<tmpListCard->nbElem; j++)
				{
					tmpCard = (Card*) list_data(tmpListCard);
					list_insert(postmanListCard, tmpCard);
					list_removeNext(tmpListCard);
				}
			}
		// ***************************************************************************
			list_next(workshopList);
		}
		pthread_mutex_unlock(&mutexListWorkshop);	 
		
		// Wake up the LB for cards transmission
		pthread_cond_signal(&condLBWakeUp);
	
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
	Card *tmp_card;
	void* recup_tmp;
	
	
	printf("===> entree stop Launching_board\n");
	while(1)
	{
		pthread_mutex_lock(&mutexPostman_listcard); 	
		
		// Wait for Postman signal
		pthread_cond_wait(&condLBWakeUp, &mutexPostman_listcard);
		  
		sleep(1);		
		//on recupère les cartes magnetiques de postmanListCard et on envoi les ordres de fabrication (reveils workshops) 
		
		//check la liste envoyée par l'homme flux
		//while(Postman_listCard != NULL)
		//{
			//check toutes les cartes de l'homme flux
			//recup_tmp = getLastElementData(Postman_listCard);
			//list_RemoveLastElem (Postman_listCard);

			//ajoute à sa liste toutes les cartes magnetiques
			//list_insertHead(LB_listCard, recup_tmp);
		//}
		
		//while
			//on recupere les données de la carte magnetique
			//recup_tmp = getLastElementData(LB_listCard);
			//tmp_card = (Card*) recup_tmp;
			//tmp_card->ref_piece;
		//fin while
		pthread_mutex_unlock(&mutexPostman_listcard);
		
		// Wake up the LB for receiving all cards
		pthread_cond_signal(&condPostmanWakeUp);		

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		if(nbProductsFinished == nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
		pthread_mutex_unlock(&stopMutex);
	}
	printf("<=== sortie Launching_board\n");
	
	// Frees and exit
	pthread_exit(NULL);
}


void* Supplier_Step_thread_fct(void* arg)
{ 	
	int* numberThread = (int*) arg;
	Workshop *supplier = (Workshop*) malloc(sizeof(Workshop));

	supplier = initWorkshop(supplier, "Supplier", *numberThread, -1);	
	list_insert(workshopList, supplier);
	
	sleep(2);	
	//print_Card(supplier->refCard);
	//print_Workshopstar(supplier);
	//list_print_Container(supplier->stock.listContainer);
	
	printf("===> entree stop %d\n",	0);
	while(1)
	{
		sleep(2 + *numberThread);

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished == nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
		pthread_mutex_unlock(&stopMutex);
	}
	printf("<=== sortie %d\n", 0);
	
	/*while(1)
	{
		//reveil du supplier par le tableau de lancement
		pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread])
		
		//met les pieces fabriquées dans un container destiné au poste en aval
		while(supplier->containerToSend.nbPieces != nbPieceByContainer)
		{
			//fabrication piece
			usleep(200);
			supplier->containerToSend.nbPieces++;
		}

		//envoi le container (ajoute containerToSend à la fin du stock des container du poste en aval)
		if(supplier->containerToSend.nbPieces == nbPieceByContainer)
		{
			list_insert(supplier->containerToSend, workshopEnAval->stock.listContainer);
		}

	}
	*/
	
	// Frees and exit
	list_delete(&(supplier->stock.listContainer));
	list_delete(&(supplier->bal.listCard));
	free(supplier);
	
	pthread_exit(NULL);
}


void* Middle_Step_thread_fct(void* arg)
{
	int* numberThread = (int*) arg;
	Workshop *workshop = (Workshop*) malloc(sizeof(Workshop));

	workshop = initWorkshop(workshop, "Workshop", *numberThread, 0);	
	list_insert(workshopList, workshop);
	
	sleep(2 + *numberThread);	
	//print_Card(workshop->refCard);
	//print_Workshopstar(workshop);
	//list_print_Container(workshop->stock.listContainer);
	
	printf("===> entree stop %d\n", *numberThread);
	while(1)
	{
		//reveil du workshop par le tableau de lancement
		//pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread])

		//prend un container du stock si actualcontainer = 0;	
		//if(workshop->actualUsedContainer.nbPieces == 0)
		//{
			//list_first(stock.listContainer);
			//tmpContainer = (Container*) list_data(stock.listContainer);
			//actualUsedContainer = *tmpContainer;
			//list_removeFirst(stock.listContainer);
		//}
		
		//consomme le container
		//workshop->actualUsedContainer.nbPieces--;

		//si une piece a ete prise dans le container
		//if(workshop->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		//{
			// Mettre carte dans la boite aux lettres
			//list_insert(workshop->actualUsedContainer.magneticCard, workshop->bal.listCard);
		//}
		
		//met les pieces fabriquées dans un container destiné au poste en aval
		//while(workshop->containerToSend.nbPieces != nbPieceByContainer)
		//{
			//fabrication piece
			//usleep(200);
			//workshop->containerToSend.nbPieces++;
		//}

		//envoi le container (ajoute containerToSend à la fin du stock des container du poste en aval)
		//if(workshop->containerToSend.nbPieces == nbPieceByContainer)
			//list_insert(workshop->containerToSend, workshopEnAval->stock.listContainer);

		//Envoi du container vide au poste en amont !
		//if(workshop->actualUsedContainer.nbPieces == 0)
			//list_insert(workshop->actualUsedContainer, workshopEnAmont->stock.listContainer);
		
	
		sleep(2 + *numberThread);	

		// Protection by mutex because of nbProductsFinished multiple access 
		pthread_mutex_lock(&stopMutex);
		
		if(nbProductsFinished == nbProductsWanted)
		{
			pthread_mutex_unlock(&stopMutex);
			break;
		}
		pthread_mutex_unlock(&stopMutex);
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
	int* numberThread = (int*) arg;
	Workshop *finalProduct = (Workshop*) malloc(sizeof(Workshop));
	
	finalProduct = initWorkshop(finalProduct, "Final_product", *numberThread, 1);	
	list_insert(workshopList, finalProduct);
	
	sleep(2 + nbMiddleStep+1);	
	//print_Card(finalProduct->refCard);
	//print_Workshopstar(finalProduct);
	//list_print_Container(finalProduct->stock.listContainer);

	printf("===> entree stop %d\n", nbMiddleStep+1);
	while(1)
	{
		//reveil du producteur final par le tableau de lancement
		//pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread])

		//prend un container du stock si actualcontainer = 0;	
		//if(finalProduct->actualUsedContainer.nbPieces == 0)
		//{
			//list_first(stock.listContainer);
			//tmpContainer = (Container*) list_data(stock.listContainer);
			//actualUsedContainer = *tmpContainer;
			//list_removeFirst(stock.listContainer);
		//}

		// ici prendre une piece dans le container
		//finalProduct->actualUsedContainer.nbPieces--;

		//si une piece a ete prise dans le container
		//if(finalProduct->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		//{
			// Mettre carte dans la boite aux lettres
			//list_insert(finalProduct->actualUsedContainer.magneticCard, finalProduct->bal.listCard);
			
		//}	
		//fabrication de la piece
		//usleep(200);

		//Envoi du container vide au poste en amont !
		//if(finalProduct->actualUsedContainer.nbPieces == 0)
			//list_insert(finalProduct->actualUsedContainer, workshopEnAmont->stock.listContainer);
		
		//printf("1 Piece finale produite");
		//nbPieceDemand++;
	
		sleep((nbMiddleStep+1)/2);
		
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
	}
	printf("<=== sortie %d\n", nbMiddleStep+1);


	// Frees and exit
	list_delete(&(finalProduct->stock.listContainer));
	list_delete(&(finalProduct->bal.listCard));
	free(finalProduct);
	
	pthread_exit(NULL);
}


