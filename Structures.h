#ifndef STRUCTURES_H
#define	STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include "List.h"
//#include <string.h>
//#include <semaphore.h>
//#include <sys/stat.h>
//#include <fcntl.h>


#define nbMiddleStep 2
#define nbMutex nbMiddleStep+1
#define nbCond nbMiddleStep+1
#define nbPieceByContainer 2
//#define nbSem nbMiddleStep+1


/* Global variables */
//sem_t *semTab[nbSem];
pthread_t threadTab[nbMiddleStep];
pthread_mutex_t mutexTab[nbMutex];
pthread_cond_t condTab[nbCond];
int nbProductsWanted;


typedef struct {
	char* workshop_name;
	int nbmax_pieces_container;
	char* ref_piece;
	char* designation_piece;
	int num_workshop_supplier;
	int num_order;
} Card;


typedef struct {
	//carte magnetique avec une reference
	Card magnetic_card;
	int nB_pieceByContainer;
}Container;


//un stock est situé à proximité de chaque poste
//un stock est constitué de plusieurs conteneur
typedef struct {
	int nB_container;
	list *Stock_listContainer;
}Stock;



typedef struct {
	list *BAL_listCard;
} BAL;


typedef struct {
	list *LB_listCard;
}LB; //Launching Board


typedef struct {
	BAL Bal;
	Stock stock;
	char* name;

} Workshop;

#endif 	/* STRUCTURES_H */
