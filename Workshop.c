#include "Workshop.h"

/* fonction à utiliser
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
	while(1)
	{
		pthread_mutex_lock(&mutexTab[1]); 	  		// P

		//check toutes les BAL
		//recup_cm = getLastElementData(BALlistCard);
		//list_RemoveLastElem (BAL_listCard);

		//ajoute à sa liste toutes les cartes magnetiques
		//list_insertHead(Postman_listCard, recup_cm);

		pthread_mutex_unlock(&mutexTab[1]); 	  		// V 
	}
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
	pthread_cond_wait(&condTab[1], &mutexTab[1]);	
	//check la liste envoyée par l'homme flux
	while(1)
	{
		pthread_mutex_lock(&mutexTab[2]); 	  		// P 
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
		pthread_mutex_unlock(&mutexTab[2]); 	  		// V 
	}
	// On ordonne au fournisseur de reaprovisionner le poste de la carte
	pthread_cond_signal(&condTab[2]);

}

void* Supplier_Step_thread_fct(void* arg)
{ 	
	int i;
	while(1)
	{
		pthread_mutex_lock(&mutexTab[0]); 	  		/* P */
		
		// Wait for a fabrication order
		pthread_cond_wait(&condTab[2], &mutexTab[2]);	

		// Create new product
		for(i=0; i<nbPieceByContainer; i++)
		{
			sleep(5);
			printf("Supplier : One piece created\n");
		}
		
		printf("Supplier : Container ready\n");
		
		pthread_mutex_unlock(&mutexTab[0]); 		/* V */
	}
}


void* Middle_Step_thread_fct(void* arg)
{
	int* numberThread = (int*) arg;
	
	/*
	while(1)
	{
		// num des conditions, mutex = numberThread +1
	}
	*/
}

void* Final_Product_thread_fct(void* arg)
{
	int* lastNumber = (int*) arg;
	
	/*
	while(1)
	{
	
	}
	*/
}



