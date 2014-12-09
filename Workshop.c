#include "Workshop.h"

/* fonction à utiliser
	int pthread_mutex_init(&mutexTab[NbMutex]);
	int pthread_mutex_cond(&condTab[NbCond], &mutexTab[NbMutex]);

	int pthread_mutex_lock(&mutexTab[NbMutex]);
	int pthread_mutex_unlock(&mutexTab[NbMutex]);
*/


void* Launching_board_thread_fct(void* arg)
{
	//reveil poste en amont afin qu'il lui refournisse en pieces dont la carte magnetique détient la référence
}


/*
Un homme flux récupère régulièrement les cartes aux différents postes de montage et les apporte à l’atelier en amont.
*/
void* Postman_thread_fct(void* arg)
{
	
}

void* Supplier_Step_thread_fct(void* arg)
{ 	
	int i;
	
	while(1)
	{
		pthread_mutex_lock(&mutexTab[0]); 	  		/* P */
		
		// Wait for a fabrication order
		pthread_cond_wait(&condTab[0], &mutexTab[0]);	

		// Create new product
		for(i=0; i<nbPieceByContainer; i++)
		{
			sleep(5);
			printf("Supplier : One piece created\n");
		}
		
		printf("Supplier : Container ready\n");
		//sem_post(semTab[0]); 	/* V */
		
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
	
		
}



