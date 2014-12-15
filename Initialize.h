#ifndef INITIALIZE_H
#define	INITIALIZE_H

#include "Structures.h"

void initListPossibleCard(char**,char**,char**,char**, int*);
Workshop* initMiddleStep(Workshop*, int);

BAL initBAL();
Stock initStock();
Card initCard();
Container initContainer();
Workshop* initWorkshop(Workshop*, char*, int);

char* concatStringInt(char*, int);

#endif /* INITIALIZE_H */
