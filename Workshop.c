/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#include "Workshop.h"

/* fonctions à utiliser
	int pthread_mutex_init(&mutexTab[NbMutex]);
	int pthread_mutex_cond(&condTab[NbCond], &mutexTab[NbMutex]);
	int pthread_mutex_lock(&mutexTab[NbMutex]);
	int pthread_mutex_unlock(&mutexTab[NbMutex]);
void list_insertHead(list*, void*);	/* Insert an element at the top of the list 
void* getLastElementData(list*);	/* return the last element's data 
void list_RemoveLastElem (list*);	/* Remove the last element of the list 
*/


/*
Un homme flux récupère régulièrement les cartes aux différents postes de montage et les apporte à l’atelier en amont.
*/
void* Postman_thread_fct(void* arg)
{
	void* recup_cm; //carte magnetique
	
	/*while(1)
	{
		pthread_mutex_lock(&mutexTab[1]); 	  		

		//check toutes les BAL
		//recup_cm = getLastElementData(BALlistCard);
		//list_RemoveLastElem (BAL_listCard);

		//ajoute à sa liste toutes les cartes magnetiques
		//list_insertHead(Postman_listCard, recup_cm);

		pthread_mutex_unlock(&mutexTab[1]); 	  		
	}*/
	//envoi ça liste au Tableau de lancement
	pthread_cond_signal(&condTab[1]);	
	
}


/*
Tableau de lancement reveil poste en amont afin qu'il refournisse en pieces la carte magnetique détient la référence
*/
void* Launching_board_thread_fct(void* arg)
{
	Card *tmp_card;
	void* recup_tmp;
	
	/*pthread_cond_wait(&condTab[1], &mutexTab[1]);	
	//check la liste envoyée par l'homme flux
	while(1)
	{

		while(Postman_listCard != NULL)
		{
			//check toutes les cartes de l'homme flux
			//recup_tmp = getLastElementData(Postman_listCard);
			//list_RemoveLastElem (Postman_listCard);

			//ajoute à sa liste toutes les cartes magnetiques
			//list_insertHead(LB_listCard, recup_tmp);
		}
		
		//while
		//on recupere les données de la carte magnetique
		//recup_tmp = getLastElementData(LB_listCard);
		//tmp_card = (Card*) recup_tmp;
		//tmp_card->ref_piece;
		//fin while
	}
	*/
}


void* Supplier_Step_thread_fct(void* arg)
{ 	
	int* nb = (int*) arg;
	int i;
	//Workshop *supplier = (Workshop*) malloc(sizeof(Workshop));
	
	//supplier = initWorkshop("Supplier", *nb);
	
	
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
	pthread_exit(NULL);
}


void* Middle_Step_thread_fct(void* arg)
{
	int* numberThread = (int*) arg;
	Workshop *workshop = (Workshop*) malloc(sizeof(Workshop));
	Container **container = (Container**) malloc(nbContainerByStock*sizeof(Container*));
	//Container* tmpContainer;
	workshop = initWorkshop(workshop, "Workshop", *numberThread, container);
	
	printf("toto\n");


	/*
	while(1)
	{
		//reveil du workshop par le tableau de lancement
		pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread])

		//prend un container du stock si actualcontainer = 0;	
		if(workshop->actualUsedContainer.nbPieces == 0)
		{
			list_first(stock.listContainer);
			tmpContainer = (Container*) list_data(stock.listContainer);
			actualUsedContainer = *tmpContainer;
			list_removeFirst(stock.listContainer);
		}
		
		//consomme le container
		workshop->actualUsedContainer.nbPieces--;

		//si une piece a ete prise dans le container
		if(workshop->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		{
			// Mettre carte dans la boite aux lettres
			list_insert(workshop->actualUsedContainer.magneticCard, workshop->bal.listCard);
			
		}

		
		//met les pieces fabriquées dans un container destiné au poste en aval
		while(workshop->containerToSend.nbPieces != nbPieceByContainer)
		{
			//fabrication piece
			usleep(200);
			workshop->containerToSend.nbPieces++;
		}

		//envoi le container (ajoute containerToSend à la fin du stock des container du poste en aval)
		if(workshop->containerToSend.nbPieces == nbPieceByContainer)
		{
			list_insert(workshop->containerToSend, workshopEnAval->stock.listContainer);
		}

		//Envoi du container vide au poste en amont !
		if(workshop->actualUsedContainer.nbPieces == 0)
		{
			list_insert(workshop->actualUsedContainer, workshopEnAmont->stock.listContainer);
		}
		
	}
	*/
	
	pthread_exit(NULL);
}


void* Final_Product_thread_fct(void* arg)
{
	int* lastNumber = (int*) arg; //?
	int* numberThread = (int*) arg;
	Workshop *finalProduct = (Workshop*) malloc(sizeof(Workshop));
	Container **container = (Container**) malloc(nbContainerByStock*sizeof(Container*));
	
	//Container* tmpContainer;

	//finalProduct = initFinalProduct(finalProduct, "Final Product", *numberThread, container);	

	/*
	while(1)
	{
		//reveil du producteur final par le tableau de lancement
		pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread])

		//prend un container du stock si actualcontainer = 0;	
		if(finalProduct->actualUsedContainer.nbPieces == 0)
		{
			list_first(stock.listContainer);
			tmpContainer = (Container*) list_data(stock.listContainer);
			actualUsedContainer = *tmpContainer;
			list_removeFirst(stock.listContainer);
		}

		// ici prendre une piece dans le container
		finalProduct->actualUsedContainer.nbPieces--;

		//si une piece a ete prise dans le container
		if(finalProduct->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		{
			// Mettre carte dans la boite aux lettres
			list_insert(finalProduct->actualUsedContainer.magneticCard, finalProduct->bal.listCard);
			
		}	
		//fabrication de la piece
		usleep(200);

		//Envoi du container vide au poste en amont !
		if(finalProduct->actualUsedContainer.nbPieces == 0)
		{
			list_insert(finalProduct->actualUsedContainer, workshopEnAmont->stock.listContainer);
		}
		printf("1 Piece finale produite");
		nbPieceDemand++;


	}*/
	
	pthread_exit(NULL);
}






	//char *wsName, *txt;

	/* Allocations */
	//txt = (char*) malloc(5*sizeof(char));
	
	/*for(i=1; i<nbMiddleStep; i++)
	{
		//Workshop's name 
		sprintf(txt, "ws");
		sprintf(nbName, "%d", i);
		wsName = strcat(txt, nbName);
	}*/
	
	//list* testlist = list_new();
