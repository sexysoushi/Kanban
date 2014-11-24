/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara <= Espece de pas galant !!
 * Last modification : November 20, 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include "structures.h"

/*Idées : */
//Donc pour les semaphores:
//- un mutex
//- quantité d'ordre
//- carte magnetique

sem_t* mutex;
sem_t* quantite;
sem_t* carte;

//int : nb produits finis (Ressources Critiques)
#define nbProdFinis 0


/* Homme flux (thread) récupere aléatoirement des :*/
void HommeFlux()
{
	if(!fork())
	{
		pintf("etiquettes recuperees :");
		attendre(3);
		sem_wait(mutex); //P

		sem_post(mutex); //V
	}
}

/* Etiquette/carte (thread) contient ordre de fabrication*/

/* Disposé dans un tableau de lancement (thread)*/

/* Point de départ => stock de constituants (thread)*/

//qu'en penses tu ?

void traitantSIGINT(int s)
{
	exit(-1);
}

int main(int argc, char* argv[])
{
	
	signal(SIGINT,traitantSIGINT);

	return 0;
}
