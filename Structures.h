#ifndef STRUCTURES_H
#define	STRUCTURES_H

typedef struct {
	char* workshop_name;
	int nbmax_pieces_container;
	char* ref_piece;
	char* designation_piece;
	int num_workshop_supplier;
	int num_order;
} Card;

typedef struct {
	Card magnetic_card;
	int nb_pieces;
}Container;

typedef struct {
	int nb_container;
	Container container;
}Stock;

typedef struct {
	//cardList cl;
} BAL;

typedef struct {
	BAL Bal;
	Stock stock;
	char* name;
	
} Workshop;


#endif 	/* STRUCTURES_H */
