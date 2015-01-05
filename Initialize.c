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
	st.listContainer = NULL;
	
	return st;
}


Stock initStockCard(Card* c)
{
	Stock st;
	int i;
	Container* tmpContainer;
	
	st.nbContainer = nbContainerByStock;
	st.listContainer = list_new();
	
	for(i=0; i<st.nbContainer; i++)
	{
		tmpContainer = initContainer(c);
		list_insert(st.listContainer, tmpContainer);
	}
	
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


Card* initCardstar()
{
	Card* cd = (Card*) malloc(sizeof(Card));;
	cd->workshopName = NULL;
	cd->nbMaxPiecesContainer = 0;
	cd->refPiece = NULL;
	cd->designationPiece = NULL;
	cd->nameWorkshopSupplier = NULL;
	cd->numOrder = 0;
	return cd;
}

Container initContainerWorkshop()
{
	Container cont;
	cont.nbPieces = 0;
	cont.magneticCard = initCardstar();
	return cont;
}


Container* initContainer(Card* c)
{
	Container* cont = (Container*) malloc(sizeof(Container));
	cont->nbPieces = nbPieceByContainer;
	cont->magneticCard = c;
	return cont;
}

Workshop* initWorkshop(Workshop* ws, char* s, int number, int boolMiddleStep)
{
	int i;
	Card* tmpCard;
	char* tmpChar;
	
	ws->name = concatStringInt(s, number);
	ws->bal = initBAL();
	ws->actualUsedContainer = initContainerWorkshop();
	
	// Protection by mutex because of referenceListCard multiple access 
	pthread_mutex_lock(&initCardRef);
	
	/* find the good card for the stock */
	// supplier & middle_step
	if(boolMiddleStep <= 0)	
	{
		tmpCard = (Card*) list_seekCardName_voidstar(ws->name, referenceListCard);
		/* Assignement workshop refCard */
		ws->refCard = *tmpCard;	

		// Just Supplier
		if(boolMiddleStep < 0)
			ws->stock = initStock();
	}
	// middle_step & finalProduct
	if(boolMiddleStep >= 0)	
	{
		// Just Final Product
		if(boolMiddleStep > 0)
		{
			//no reference Card
			ws->refCard = initCard();
		
			tmpChar = concatStringInt("Part", nbMiddleStep+1);
		}
		// Just Middle_step
		else
			tmpChar = concatStringInt("Part", number);
			
		// Put the good refCard in the stock's containers
		tmpCard = (Card*) list_seekCardDesignation_voidstar(tmpChar, referenceListCard);
		
		//Stock initilization
		ws->stock = initStockCard(tmpCard);
	}
	
	ws->containerToSend = initContainer(&ws->refCard);
	ws->containerToSend->nbPieces = 0;
	
	pthread_mutex_unlock(&initCardRef);
		
	return ws;
}


