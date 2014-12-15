/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#include "Initialize.h"


void initListPossibleCard(char** cardWorkshopName, char** cardRefPiece, char** cardDesignationPiece, char** cardNameWorkshopSupplier, int* cardNumOrder)
{
	int i;
	Card *newCard[nbDiffentCard];
	
	printf("Global initilization of the global list of possible <Card> :\n");
	
	for(i=0; i<nbDiffentCard; i++)
	{
		newCard[i] = (Card*) malloc(sizeof(Card));
		newCard[i]->workshopName = cardWorkshopName[i];
		newCard[i]->nbMaxPiecesContainer = nbPieceByContainer;
		newCard[i]->refPiece = cardRefPiece[i];
		newCard[i]->designationPiece = cardDesignationPiece[i];
		newCard[i]->nameWorkshopSupplier = cardNameWorkshopSupplier[i];
		newCard[i]->numOrder = cardNumOrder[0];
	
		list_insert(referenceListCard, newCard[i]);
	}
}



Workshop* initMiddleStep(Workshop *W, int num)
{
	W = initWorkshop(W, "Workshop", num);
	
	/* Protection by mutex because of referenceListCard multiple access */
	pthread_mutex_lock(&initCardRef);
	Card* tmpCard = (Card*) list_seek_voidstar(W->name, referenceListCard);
	pthread_mutex_unlock(&initCardRef);
	
	W->refCard = *tmpCard;
	
	return W;
}


/* Return the string "s" with number "nb" at the end */
char* concatStringInt(char* s, int nb)
{
	char *txt, *tmp=NULL;
	
	txt = (char*) malloc(15*sizeof(char));
	tmp = (char*) malloc(15*sizeof(char));
	sprintf(txt, "%s", s);
	sprintf(tmp, "%d", nb);
	txt = strcat(txt, tmp);
	
	free(tmp);
	tmp = NULL;
	
	return txt;
}


BAL initBAL()
{
	BAL b;
	b.listCard = list_new();
	return b;	
}


Stock initStock()
{
	Stock st;
	st.nbContainer = 0;
	st.listContainer = list_new();
	return st;
}


Card initCard()
{
	Card cd;
	cd.workshopName = NULL;
	cd.nbMaxPiecesContainer = 0;
	cd.refPiece = NULL;
	cd.designationPiece = NULL;
	cd.nameWorkshopSupplier = NULL;
	cd.numOrder = 0;
	return cd;
}


Container initContainer()
{
	Container cont;
	cont.nbPieces = 0;
	cont.magneticCard = initCard();
	return cont;
}

Workshop* initWorkshop(Workshop* ws, char* s, int number)
{
	ws->name = concatStringInt(s, number);;
	ws->stock = initStock();
	ws->bal = initBAL();
	ws->actualUsedContainer = initContainer();
	ws->refCard = initCard();
	return ws;
}


LB initLB()
{
	LB lb;
	lb.listCard = list_new();
	return lb;
}


