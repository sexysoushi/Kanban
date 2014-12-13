#ifndef INITIALIZE_H
#define	INITIALIZE_H

#include "Structures.h"

list* initListPossibleCard();
BAL initBAL();
Stock initStock();
Card initCard();
Container initContainer();
Workshop initWorkshop(int);

char* concatStringInt(char*, int);

#endif /* INITIALIZE_H */
