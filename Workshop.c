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
	postmanListCard = list_new();
	
	while(1)
	{
		pthread_mutex_lock(&mutexTab[1]); 	  		
		
		//on recupère les cartes magnetiques de chaque BAL et les mets dans la liste du postman
		

		pthread_mutex_unlock(&mutexTab[1]); 	  		
	}
	//envoi ça liste au Tableau de lancement
		

	list_delete(&postmanListCard);
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
	int* numberThread = (int*) arg;
	Container* tmpContainer;
	list_insert(workshopList, supplier);
	list_insert(supplier.containerToSendList, supplier->containerToSend); //initialise un container dans la liste des containers à remplir et envoyer
	
	sleep(2);	
	print_Card(supplier->refCard);
	print_Workshopstar(supplier);
	list_print_Container(supplier->stock.listContainer);
	/*
	while(1)
	{
		printf("===> entree stop %d\n",	0);
		break;
	}
	printf("<=== sortie %d\n", 0);
	*/

	while(1)
	{
		//reveil du supplier par le tableau de lancement
		pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread]);

		//met les pieces fabriquées dans un container du stock destiné au poste en aval
		while(list_first(workshop->stock.listContainer).nbPieces != nbPieceByContainer)
		{
			//fabrication piece
			usleep(200);
			list_first(workshop->stock.listContainer).nbPieces++;
			
		}

	
	}
	
	pthread_exit(NULL);
}


void* Middle_Step_thread_fct(void* arg)
{
	int* numberThread = (int*) arg;
	Container* tmpContainer;
		
	list_insert(workshopList, workshop);
	
	sleep(2+*numberThread);	
	print_Card(workshop->refCard);
	print_Workshopstar(workshop);
	list_print_Container(workshop->stock.listContainer);
	/*
	while(1)
	{
		printf("===> entree stop %d\n", *numberThread);
		sleep((*numberThread)/2);
		break;
	}
	printf("<=== sortie %d\n", *numberThread);
	*/
	while(1){
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

		//met les pieces fabriquées dans un container du stock destiné au poste en aval
		while(list_first(finalProduct->stock.listContainer).nbPieces != nbPieceByContainer)
		{
			//fabrication piece
			usleep(200);
			list_first(finalProduct->stock.listContainer).nbPieces++;
			
		}

		//Envoi du container vide au poste en amont !
		if(workshop->actualUsedContainer.nbPieces == 0)
		{
			list_insert(supplier->stock.listContainer, workshop->actualUsedContainer);
		}
		
	}
	
	list_delete(&(workshop->stock.listContainer));
	//list_delete(&(workshop->bal.listCard));
	free(workshop);
		
	pthread_exit(NULL);
}


void* Final_Product_thread_fct(void* arg)
{
	int* numberThread = (int*) arg;
	Container* tmpContainer;	
	list_insert(workshopList, finalProduct);
	
	sleep(2+nbMiddleStep+1);	
	print_Card(finalProduct->refCard);
	print_Workshopstar(finalProduct);
	list_print_Container(finalProduct->stock.listContainer);
/*
	while(1)
	{
		printf("===> entree stop %d\n", nbMiddleStep+1);
		sleep((nbMiddleStep+1)/2);
		break;
	}
	printf("<=== sortie %d\n", nbMiddleStep+1);
*/
	while(1)
	{
		//reveil du producteur final par le tableau de lancement
		pthread_mutex_wait(&condTab[numberThread], &mutexTab[numberThread]);

		// si actualcontainer = 0;	
		if(finalProduct->actualUsedContainer.nbPieces == 0)
		{
			//Envoi du container vide au poste en amont !
			list_insert(workshop->stock.listContainer, finalProduct->actualUsedContainer);
			
			//prend un container plein du stock
			list_first(finalProduct->stock.listContainer);
			tmpContainer = (Container*) list_data(finalProduct->stock.listContainer);
			finalProduct->actualUsedContainer = *tmpContainer;
			list_removeFirst(finalProduct->stock.listContainer);
		}

		// ici prendre une piece dans le container
		finalProduct->actualUsedContainer.nbPieces--;

		//si une piece a ete prise dans le container
		if(finalProduct->actualUsedContainer.nbPieces == nbPieceByContainer-1)
		{
			// Mettre carte dans la boite aux lettres
			list_insert(finalProduct->bal.listCard, finalProduct->actualUsedContainer.magneticCard);
			
		}	
		//fabrication de la piece
		usleep(200);
		printf("1 Piece finale produite");
		nbProductsWantedFinish++;

	}
	
	list_delete(&(finalProduct->stock.listContainer));
	//list_delete(&(workshop->bal.listCard));
	free(finalProduct);
	
	pthread_exit(NULL);
}


