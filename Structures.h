/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#ifndef STRUCTURES_H
#define	STRUCTURES_H


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "List.h"


#define nbMiddleStep 4	// at least 1
#define nbDiffentCard nbMiddleStep+1
#define nbMutex nbMiddleStep+2
#define nbCond nbMiddleStep+2
#define nbPieceByContainer 2
#define nbContainerByStock 2

#define minusFabricationTime 4

/* Global variables */
pthread_t t1, t2, t3, t4;	// 4 indispensable threads 
pthread_t threadTab[nbMiddleStep]; 	// Threads for dynamic number of workshop

pthread_mutex_t initCardRef, mutexListWorkshop;
pthread_mutex_t stopMutex;
pthread_mutex_t mutexPostman_listcard;
pthread_mutex_t mutexTab[nbMutex];	

pthread_cond_t condTab[nbCond];	
pthread_cond_t condPostmanWakeUp, condLBWakeUp;

// Declaration in global so we can free them when SIGINT signal is received
int *tabNumber;
char **cardWorkshopName, **cardRefPiece, **cardDesignationPiece, **cardNameWorkshopSupplier;
list* postmanListCard;	
list* referenceListCard;
list* workshopList;

int nbProductsWanted;	// Client's asking
int nbProductsFinished;	//Client's pieces finish


typedef struct {
	char* workshopName;
	int nbMaxPiecesContainer;
	char* refPiece;
	char* designationPiece;
	char* nameWorkshopSupplier;
	int numOrder;
} Card;


typedef struct {
	Card* magneticCard;
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
	Container actualUsedContainer, *containerToSend;
	Card refCard;
	char* name;
} Workshop;

/*
typedef struct {
	list *listCard;
}LB; //Launching Board
*/

/*Functions implemented at the beginning of the main file */
void applicateWhenSIGINT(int);
void error(const char*);

#endif 	/* STRUCTURES_H */

