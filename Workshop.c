#include "Workshop.h"

void* Launching_board_thread_fct(void* arg)
{

}

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



