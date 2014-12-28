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
					list_removeFirst(tmpListCard);
				}
			}
		// ***************************************************************************
			list_next(workshopList);
		}
		pthread_mutex_unlock(&mutexListWorkshop);	 
		
		printf("before signal postman\n");
		// Wake up the LB for cards transmission
		pthread_cond_signal(&condLBWakeUp);
		printf("after signal postman !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	
		printf("before wait postman\n");
		// Wait for LB signal
		pthread_cond_wait(&condPostmanWakeUp, &mutexPostman_listcard);
		printf("after wait postman\n");
		
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
	int i,j;
	Card *tmpCard;
	
	
	printf("===> entree stop Launching_board\n");
	while(1)
	{
		pthread_mutex_lock(&mutexPostman_listcard); 	
		
		// Wait for Postman signal
		printf("before wait LB !!!!!!!!!!!!!!!!!!!!!!!!\n");
		pthread_cond_wait(&condLBWakeUp, &mutexPostman_listcard);
		printf("after wait LB\n");
		sleep(1);	
		
		list_first(postmanListCard);
		
		for(i=0; i<postmanListCard->nbElem; i++)
		{
			tmpCard = (Card*) list_data(postmanListCard);
			
			for(j=1; j<(nbMiddleStep+1); j++)
			{
				if(tmpCard->workshopName == concatStringInt("Supplier", j))
				{
					printf("before signal LB\n");
					// here with just one supplier j-1 must be equal to 0
					pthread_mutex_lock(&mutexTab[j-1]);
					pthread_cond_signal(&condTab[j-1]);
					pthread_mutex_unlock(&mutexTab[j-1]);
					printf("after signal LB\n");
					break;
				}
				
				if(tmpCard->workshopName == concatStringInt("Workshop", j))
				{
					printf("before signal LB\n");
					pthread_mutex_lock(&mutexTab[j]);
					pthread_cond_signal(&condTab[j]);
					pthread_mutex_unlock(&mutexTab[j]);
					printf("after signal LB : sent to %s (j = %d)\n", tmpCard->workshopName, j);
					break;
				}
			}
					
			list_removeFirst(postmanListCard);
			list_next(postmanListCard);
		}
			
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
	
	sleep(1);	
	//print_Card(supplier->refCard);
	//print_Workshopstar(supplier);
	//list_print_Container(supplier->stock.listContainer);
	
	printf("===> entree stop %d\n",	0);
	while(1)
	{
		pthread_mutex_lock(&mutexTab[0]);
		
		//reveil du workshop par le tableau de lancement
		pthread_cond_wait(&condTab[0], &mutexTab[0]);
		
			
		//met les pieces fabriquées dans un container destiné au poste en aval
		//while(supplier->containerToSend.nbPieces != nbPieceByContainer)
		//{
			//fabrication piece
			//usleep(200);
			//supplier->containerToSend.nbPieces++;
		//}

		//envoi le container (ajoute containerToSend à la fin du stock des container du poste en aval)
		//if(supplier->containerToSend.nbPieces == nbPieceByContainer)
		//{
			//list_insert(supplier->containerToSend, workshopEnAval->stock.listContainer);
		//}
		
		pthread_mutex_lock(&mutexTab[0]);
		sleep(1);

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
	
	sleep(1);	
	//print_Card(workshop->refCard);
	//print_Workshopstar(workshop);
	//list_print_Container(workshop->stock.listContainer);
	
	printf("===> entree stop %d\n", *numberThread);
	while(1)
	{	
		pthread_mutex_lock(&mutexTab[*numberThread]);
		
		printf("%s : before Wait \n", workshop->name);
		//reveil du workshop par le tableau de lancement
		pthread_cond_wait(&condTab[*numberThread], &mutexTab[*numberThread]);
		printf("%s : after Wait \n", workshop->name);
		
		//prend un container du stock si actualcontainer = 0;	
		if(workshop->actualUsedContainer.nbPieces == 0)
			takeNewContainerFromStock(workshop);
		
		//consomme le container
		workshop->actualUsedContainer.nbPieces--;

		//si une piece a ete prise dans le container
		if(workshop->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		{
			// Mettre carte dans la boite aux lettres
			list_insert(workshop->bal.listCard, (void*) workshop->actualUsedContainer.magneticCard);
			printf("%s : Carte dans BAL \n", workshop->name);
		}
		
		printf("%s : Piece's creation in progress...\n", workshop->name); 
		sleep(4);
		
		//met la piece fabriquée dans un container destiné au poste en aval
		
		workshop->containerToSend.nbPieces++;	

		if((*numberThread+1) == (nbMiddleStep+1))
		{
			pthread_mutex_lock(&mutexTab[*numberThread +1]);
			pthread_cond_signal(&condTab[*numberThread +1]);
			pthread_mutex_unlock(&mutexTab[*numberThread +1]);
		}
		//envoi le container dans le stock du post en aval
		/*if(workshop->containerToSend.nbPieces == nbPieceByContainer)
		{
			list_insert(workshopEnAval->stock.listContainer, workshop->containerToSend);
			workshop->containerToSend.nbPieces = 0;
		}*/

		//Envoi du container vide au poste en amont !
		//if(workshop->actualUsedContainer.nbPieces == 0)
			//list_insert(workshop->actualUsedContainer, workshopEnAmont->stock.listContainer);
		
		pthread_mutex_unlock(&mutexTab[*numberThread]);	
		sleep(1);	

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
	
	void* tmpElemVoidstar;
	Workshop *finalProduct = (Workshop*) malloc(sizeof(Workshop));
	
	finalProduct = initWorkshop(finalProduct, "Final_product", *numberThread, 1);	
	list_insert(workshopList, finalProduct);
	
	sleep(1);	
	//print_Card(finalProduct->refCard);
	//print_Workshopstar(finalProduct);
	//list_print_Container(finalProduct->stock.listContainer);

	printf("===> entree stop %d\n", nbMiddleStep+1);
	while(1)
	{
		pthread_mutex_lock(&mutexTab[nbMiddleStep+1]);
	
		//prend un container du stock si actualcontainer = 0;	
		if(finalProduct->actualUsedContainer.nbPieces == 0)
			takeNewContainerFromStock(finalProduct);
		
		//print_Workshopstar(finalProduct);
		//list_print_Container(finalProduct->stock.listContainer);
		//print_Containerstar(tmpContainer);
		

		// ici prendre une piece dans le container
		finalProduct->actualUsedContainer.nbPieces--;

		//si prise de la premiere piece du conteneur
		if(finalProduct->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		{
			// Mettre carte dans la boite aux lettres 
			list_insert(finalProduct->bal.listCard, (void*) finalProduct->actualUsedContainer.magneticCard);
			printf("%s : Carte dans BAL \n", finalProduct->name);
		}
		
		//fabrication de la piece
		printf("%s : Piece's creation in progress...\n", finalProduct->name); 
		sleep(4);

		//Envoi du container vide au poste en amont !
		//if(finalProduct->actualUsedContainer.nbPieces == 0)
			//list_insert(finalProduct->actualUsedContainer, workshopEnAmont->stock.listContainer);
		
		printf("%s : 1 Piece finished !\n", finalProduct->name);
		
		sleep(1);
		
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
		
		printf("%s : before Wait \n", finalProduct->name);
		//reveil du producteur final pour créer un autre pièce
		pthread_cond_wait(&condTab[nbMiddleStep+1], &mutexTab[nbMiddleStep+1]);
		pthread_mutex_unlock(&mutexTab[nbMiddleStep+1]);
		printf("%s : after Wait \n", finalProduct->name);
		
	}
	printf("<=== sortie %d\n", nbMiddleStep+1);

	// Frees and exit
	list_delete(&(finalProduct->stock.listContainer));
	list_delete(&(finalProduct->bal.listCard));
	free(finalProduct);
	
	pthread_exit(NULL);
}

void takeNewContainerFromStock(Workshop* w)
{
	Container *tmpContainer;

	list_first(w->stock.listContainer);
	tmpContainer = (Container*) list_data(w->stock.listContainer);
	w->actualUsedContainer = *tmpContainer;
	list_removeNext(w->stock.listContainer);
	w->stock.nbContainer--;
}





