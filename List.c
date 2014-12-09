#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"

list* list_new(void)
{
   	list *l = (list*) malloc(sizeof(list));

   	if (l != NULL)
   	{
      	st_elem *elem = (st_elem*) malloc(sizeof(st_elem));

		if (elem != NULL)
		{
			elem->data = NULL;
			elem->next = NULL;

			l->head = elem;
			l->tail = elem;
		}
		else
		{
		 	fprintf (stderr, "Memoire insufisante\n");
		 	exit (EXIT_FAILURE);
		}
   	}
   	else
   	{
      	fprintf (stderr, "Memoire insufisante\n");
      	exit (EXIT_FAILURE);
   	}
   	
   	return l;
}


void list_insertHead (list *l, void *data)
{
   	if (l != NULL)
   	{
      	st_elem *new_elem =(st_elem*) malloc(sizeof(st_elem));

      	if (new_elem != NULL)
      	{
         	new_elem->data = data;
         
         	new_elem->next = l->head;
         	l->head = new_elem;
      	}
      	else
      	{
         	fprintf (stderr, "Memoire insuffisante\n");
         	exit (EXIT_FAILURE);
      	}
	}
}


void* list_getLastElementData(list *l)
{	
	void* value;
	
	if (l != NULL)
	{
		st_elem *current = l->head;
			
		while(current->next != l->tail)
			current = current->next;
		
		value = current->data;
		
		return value;
	}
	return;
}


void list_RemoveLastElem (list *l)
{	
	if (l != NULL)
   	{
   		st_elem *toDel;
   		if(l->head->next != NULL)
   		{
			st_elem *current = l->head;
			
			while(current->next != l->tail)
				current = current->next;	
		
			toDel = current->next;
			current->next = NULL;
			l->tail = current;
		}
		else
		{
			toDel = l->head;
			l->head = l->tail = NULL;
		}

      	free(toDel);
      	toDel = NULL;
   	}
}


void list_delete (list *l)
{
   	if (l != NULL)
   	{
     	while (l->head != l->tail)
      		list_RemoveLastElem(l);
      		
      	free (l);
      	l = NULL;
   	}
}


void list_print_card(list* l)
{
	st_elem * currentElem = l->head;
	Card *card = currentElem->data;
	
	if(l == NULL)
		printf("La liste est vide !\n");
	else
		while(currentElem != NULL)
		{
			printf("Card number : %d \n", card->num_order);
			printf("Piece reference : %s \n", card->ref_piece);
			printf("\n");
			currentElem = currentElem->next;
		}
}
