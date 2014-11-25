#ifndef WORKSHOP_H
#define	WORKSHOP_H

#include "List.h"

void* Final_Product_thread_fct(void* arg);
void* Middle_Step_thread_fct(void* arg);
void* Supplier_Step_thread_fct(void* arg);

#endif /* WORKSHOP_H */
