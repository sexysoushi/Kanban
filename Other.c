/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */
 
#include "Other.h"
#include "Structures.h"


void print_Cardstar(Card* c)
{
	printf("%s\t\t", (char *) c->workshopName);
	printf("%d\t\t", (int) c->nbMaxPiecesContainer);
	printf("%s\t\t", (char *) c->refPiece);
	printf("%s\t\t", (char *) c->designationPiece);
	printf("%s\t\t", (char *) c->nameWorkshopSupplier);
	printf("%d\n", (int) c->numOrder);
}


void print_Card(Card c)
{
	printf("%s\t\t", c.workshopName);
	printf("%d\t\t", c.nbMaxPiecesContainer);
	printf("%s\t\t", c.refPiece);
	printf("%s\t\t", c.designationPiece);
	printf("%s\t\t", c.nameWorkshopSupplier);
	printf("%d\n", c.numOrder);
}

void print_Workshopstar(Workshop* w)
{
	printf("%s\t\t", (char *) w->name);
	printf("stock : %d\t\t", (int) w->stock.nbContainer);
	printf("%s\t\n", (char *) w->refCard.refPiece);
}

void print_Containerstar(Container* cont)
{
	printf("%d\t\t", (int) cont->nbPieces);
	print_Cardstar(cont->magneticCard);
}

/*
Workshop* findWorkshop(char *s)
{
	int j;
	Workshop* tmpWorkshop;
	
	pthread_mutex_lock(&mutexListWorkshop);
	list_first(workshopList);
	
	for(j=1; j<workshopList->nbElem-1; j++)
	{	
		tmpWorkshop = list_seekWorkshopName_voidstar(s, workshopList);
		
		return tmpWorkshop;	
	}
	
	pthread_mutex_unlock(&mutexListWorkshop);
}
*/


