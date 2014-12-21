/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */
 
#ifndef INITIALIZE_H
#define	INITIALIZE_H

#include "Structures.h"

void initListPossibleCard(char**,char**,char**,char**, int*);
//Workshop* initMiddleStep(Workshop*, int);

BAL initBAL();
Stock initStock(Card);
Card initCard();
Container* initContainer(Card);
Container initContainerWorkshop();
Workshop* initWorkshop(Workshop*, char*, int);
LB initLB();

char* concatStringInt(char*, int);

#endif /* INITIALIZE_H */
