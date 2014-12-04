#ifndef STRUCTURES_H
#define	STRUCTURES_H

//une carte magnetique
typedef struct {
	char* workshop_name;
	int nbmax_pieces_container;
	char* ref_piece;
	char* designation_piece;
	int num_workshop_supplier;
	int num_order;
} Card;

//un container possede une carte magnetique, un nb de piece
typedef struct {
	//carte magnetique avec une reference
	Card magnetic_card;
	int nB_pieceByContainer = 5;

}Container;

//un stock est situé à proximité de chaque poste
//un stock est constitué de plusieurs conteneur
typedef struct {
	int nB_container = 5;
	//tableau de container
	Container tabContainer[nB_container];

}Stock;

//chaque poste possede une BAL
typedef struct {
	//le nom du poste où elle se trouve
	//comment on recupere le nom du poste? 

	//cardList cl;
} BAL;


//peut etre pas besoin d'une struture pour ça
typedef struct {
	Card card;

}TdeL; //Tableau de Lancement

typedef struct {
	BAL Bal;
	Stock stock;
	char* name;
	List<Indications> indications;
	//une localisation
	
} Workshop;

#endif 	/* STRUCTURES_H */
