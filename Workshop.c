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
{ 	/*
	while(1)
	{
		// Réveil du fournisseur par mutex "mut1"  + condition "cond1"
		
		if(nbProductsStock < nbNeed)
		{
			//creation d'un nouveau produits
			sem_post(semTab[0]); // Opération V
		}
		else
			//endormissement sur condition "cond1"
	}
	*/
}


void* Middle_Step_thread_fct(void* arg)
{
	/*
	while(1)
	{
		sem_wait(semTab[0]); //Opération P
		
		if(produit en stock)
		{
			//fourni nb produit demande
			needProduct = false;
		}
		else
		{
			//on reveil production en amont	
			needProduct = true;
			
		}
		
		sem_post(semTab[1]); // Opération V	
	}*/
		
}

void* Final_Product_thread_fct(void* arg)
{
	/*bool needProduct = false;
	
	if(produit en stock)
	{
		//alors on sort
		pthread_exit(NULL);
	}
	else
	{
		//on reveil production en amont	
		needProduct = true;
		sem_post(semTab[0]); // Operation V	
	}
*/
}



