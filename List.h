#ifndef LIST_H
#define LIST_H

#include "Structures.h"

typedef struct elem
{
	void* data;
    struct elem* next;
} st_elem;

typedef struct st_elem
{
	st_elem *head;
	st_elem *tail;
} list;


list* list_new(); /* Create new list */
void list_insertHead(list*, void*);	/* Insert an element at the top of the list */
void* getLastElementData(list*);	/* return the last element's data */
void list_RemoveLastElem (list*);	/* Remove the last element of the list */
void list_delete(list*);	/* Delete the entire list */
void print_list_card(list*);	/* Print all the cards which are in the list */

#endif /* LIST_H */
