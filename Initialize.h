/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */
 
#ifndef INITIALIZE_H
#define	INITIALIZE_H

#include "Structures.h"

void initListPossibleCard(char**,char**,char**,char**, int*);

BAL initBAL();
Stock initStockCard(Card*);
Stock initStock();
Card initCard();
Card* initCardstar();
Container* initContainer(Card*);
Container initContainerWorkshop();
Workshop* initWorkshop(Workshop*, char*, int, int);

char* concatStringInt(char*, int);

#endif /* INITIALIZE_H */




	
