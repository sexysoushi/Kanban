#include "Initialize.h"


list* initListPossibleCard()
{
	int i;
	Card *newCard;
	newCard = (Card*) malloc(sizeof(Card));
	referenceListCard = list_new();
	
	char cardWorkshopName[][15] = {"Supplier1", "Workshop1", "Workshop2", "FinalStep1"};
	char cardRefPiece[][6] = {"S1P1", "W1P1", "W2P1", "L1P1"};
	char cardDesignationPiece[][6] = {"Part1", "Part2", "Part3", "Part4"};
	char cardNameWorkshopSupplier[][15] = {"none", "Supplier1", "Workshop1", "Workshop2"};
	int cardNumOrder[4] = {0, 1, 2, 3};
	
	printf("\nInitialisation liste de Card possibles :\n");
	
	for(i=0; i<4; i++)
	{
		newCard->workshopName = cardWorkshopName[i];
		newCard->nbMaxPiecesContainer = nbPieceByContainer;
		newCard->refPiece = cardRefPiece[i];
		newCard->designationPiece = cardDesignationPiece[i];
		newCard->nameWorkshopSupplier = cardNameWorkshopSupplier[i];
		newCard->numOrder = cardNumOrder[0];
	
		list_insert(referenceListCard, (void*) newCard);
		list_print_Card(referenceListCard);
	}
	
	free(newCard);
	newCard = NULL;
}


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
	b.listCard = NULL;
	return b;	
}


Stock initStock()
{
	Stock st;
	st.nbContainer = 0;
	st.listContainer = NULL;
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

Workshop initWorkshop(int number)
{
	Workshop ws;
	ws.name = NULL;
	ws.stock = initStock();
	ws.bal = initBAL();
	ws.actualUsedContainer = initContainer();
	ws.refCard = initCard();
	return ws;	 	
}
