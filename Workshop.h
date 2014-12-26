/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#ifndef WORKSHOP_H
#define	WORKSHOP_H

#include "Structures.h"
#include "Initialize.h"

Workshop *supplier = (Workshop*) malloc(sizeof(Workshop));
supplier = initWorkshop(supplier, "Supplier", *numberThread, -1);

Workshop *workshop = (Workshop*) malloc(sizeof(Workshop));
workshop = initWorkshop(workshop, "Workshop", *numberThread, 0);

Workshop *finalProduct = (Workshop*) malloc(sizeof(Workshop));
finalProduct = initWorkshop(finalProduct, "Final_product", *numberThread, 1);

//Tableau de lancement
void* Launching_board_thread_fct(void* arg);
//Homme flux
void* Postman_thread_fct(void* arg);
//Fournisseur
void* Supplier_Step_thread_fct(void* arg);
//Poste de travail
void* Middle_Step_thread_fct(void* arg);
//Poste final
void* Final_Product_thread_fct(void* arg);


#endif /* WORKSHOP_H */
