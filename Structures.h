#ifndef STRUCTURES_H
#define	STRUCTURES_H

typedef struct {
	char* workshop_name;
	int nbmax_pieces_container;
	char* ref_piece;
	char* piece_designation;
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



#endif 	/* STRUCTURES_H */
