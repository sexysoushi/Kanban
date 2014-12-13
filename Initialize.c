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


