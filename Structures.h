#ifndef STRUCTURES_H
#define	STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "List.h"

#define nbDiffentCard 4 
#define nbMiddleStep 1
#define nbMutex nbMiddleStep+1
#define nbCond nbMiddleStep+1
#define nbPieceByContainer 2


/* Global variables */
pthread_t t1, t2, t3, t4;	// 4 threads which can't change
pthread_t threadTab[nbMiddleStep]; 	// Threads for dynamic number of workshop
pthread_mutex_t mutexTab[nbMutex];	
pthread_cond_t condTab[nbCond];	
int nbProductsWanted;	// Client's asking
list* Postman_listCard;	
list* referenceListCard;


typedef struct {
	char* workshopName;
	int nbMaxPiecesContainer;
	char* refPiece;
	char* designationPiece;
	char* nameWorkshopSupplier;
	int numOrder;
} Card;


typedef struct {
	//carte magnetique avec une reference
	Card magneticCard;
	int nbPieces;
} Container;


//un stock est situé à proximité de chaque poste
//un stock est constitué de plusieurs conteneur
typedef struct {
	int nbContainer;
	list *listContainer;
} Stock;


typedef struct {
	list *listCard;
} BAL;


typedef struct {
	BAL bal;
	Stock stock;
	Container actualUsedContainer;
	Card refCard;
	char* name;
} Workshop;


typedef struct {
	list *listCard;
}LB; //Launching Board


#endif 	/* STRUCTURES_H */
