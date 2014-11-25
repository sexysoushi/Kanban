#include<Workshop.h>


void* Workshop_thread_fct(void* arg)
{
	bool needProduct = false;

	//Cas du produit finit
	if(arg == FinishProduct)
	{
		if(produit en stock)
		{
			//alors on sort
			pthread_exit(NULL);
		}
		else
		{
			//on reveil production en amont	
			needProduct = true;
			sem_post(semTab[0]); //V	
		}
	}

	//Cas de l'intermediaire
	if(arg == Middle)
	{
		if(needProduct)
		{
			sem_wait(semTab[0]); //P
			if(produit en stock)
			{
				//fourni nb produit demande

				needProduct = false;
			}
			else
			{
				//on reveil production en amont	
				needProduct = true;
				sem_post(semTab[1]); //V	
			}
		}
		
	}

	//Cas du fournisseur
	if(arg == Supplier)
	{
		if(needProduct)
		{
			sem_wait(semTab[1]); //P
			//fourni nb produit demande
			
			needProduct = false;
			}
		}
	}
//voilà et pour le retour on regardera ensemble, déjà regarde ça et dis moi si c'est à peut près ça ?
}
