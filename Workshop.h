#ifndef WORKSHOP_H
#define	WORKSHOP_H

#include "List.h"
#include "Structures.h"
#include <semaphore.h>

//ces variables sont initialisées dans le main
pthread_mutex_t mutex;
pthread_cond_t cond;
/* fonction à utiliser
	int pthread_mutex_init(&mutex);
	int pthread_mutex_cond(&cond, &mutex);

	int pthread_mutex_lock(&mutex);
	int pthread_mutex_unlock(&mutex);
*/


//Tableau de lancement
void* Launching_board_thread_fct(void* arg);
//Homme flux
void* Postman_thread_fct(void* arg);
//Fournisseur
void* Supplier_Step_thread_fct(void* arg);
//Poste de travail
void* Middle_Step_thread_fct(void* arg);
//Poste final
void* Final_Product_thread_fct(void* arg);

#endif /* WORKSHOP_H */
