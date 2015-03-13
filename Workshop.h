/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#ifndef WORKSHOP_H
#define	WORKSHOP_H

#include "Structures.h"
#include "Initialize.h"
#include "Other.h"

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


Workshop* takeNewContainerFromStock(Workshop*);

#endif /* WORKSHOP_H */
