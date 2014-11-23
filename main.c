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


//int : nb produits finis (Ressources Critiques)

/* Homme flux (thread) récupere aléatoirement des :*/
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
