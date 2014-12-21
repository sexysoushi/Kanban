/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"


list *list_new (void)
{
  	list *l = (list*) malloc (sizeof(list));

   	if (l)
   	{
      	item_s *listElem = (item_s*) malloc (sizeof(item_s));
      	if (listElem)
      	{
         	listElem->data = NULL;
         	listElem->next = NULL;

         	l->head= listElem;
         	l->currentElem = NULL;
         	l->nbElem = 0;
      	}
      	else
      	{
         	error("Memoire insufisante\n");
         	exit (EXIT_FAILURE);
      	}
   	}
   	else
   	{
      	error("Memoire insufisante\n");
      	exit (EXIT_FAILURE);
   	}
   	return l;
}



void list_insert (list * l, void *data)
{
   	if (l)
   	{
      	item_s *listElem = l->currentElem;
      	item_s *newElem = NULL;

      	newElem = (item_s*) malloc (sizeof(item_s));
      	if (newElem)
      	{
         	newElem->data = data;
         	if (listElem == NULL)
         	{
           		l->head->next = newElem;
            	newElem->next = NULL;
         	}
         	else
         	{
            	newElem->next = listElem->next;
            	listElem->next = newElem;
         	}
         	l->currentElem = newElem;
         	l->nbElem++;
      	}
      	else
      	{
			fprintf (stderr, "Memoire insuffisante\n");
         	exit (EXIT_FAILURE);
      	}
   	}
}



void list_removeNext (list * l)
{
   	if (l && l->currentElem)
   	{
      	item_s *listElem = l->currentElem;
      	item_s *toDel = NULL;

      	toDel = listElem->next;
      	listElem->next = toDel->next;
      	l->nbElem--;
      	free (toDel);
      	toDel = NULL;
   	}
}



void list_removeFirst (list * l)
{
   	if (l)
   	{	
      	l->currentElem = l->head;
      	list_removeNext (l);
   	}
}



void list_next (list * l)
{
   	if (l && l->currentElem)
     	l->currentElem = l->currentElem->next;
}



void *list_data (list * l)
{
   	return ((l && l->currentElem) ? l->currentElem->data : NULL);
}



void list_first (list * l)
{
   	if (l)
      	l->currentElem = l->head->next;
}



void list_last (list * l)
{
   	if (l)
  		while (l->currentElem->next != NULL)
         	list_next (l);
}



void list_delete (list ** pl)
{
   	if (pl && *pl)
   	{
      	list_first (*pl);

      	while ((*pl)->currentElem->next != NULL)
         	list_removeNext (*pl);

      	list_removeFirst (*pl);
      	free ((*pl)->currentElem);
      	(*pl)->currentElem = NULL;
      	free (*pl), *pl = NULL;
   	}
}



char* list_seek_char(char* s, list* l)
{
	char* tmp = NULL;
	if(l)
	{
		list_first(l);
		tmp =(char*) list_data(l);
		
		while(l->currentElem && strcmp(tmp,s))
		{
			list_next(l);
			tmp =(char*) list_data(l);
		}		
	}
	return tmp;
}



void* list_seek_voidstar(char* s, list* l)
{
	void *tmp = NULL;
	Card *tmpCard;
	if(l)
	{
		list_first(l);
		tmp = list_data(l);
		tmpCard = (Card*) tmp;
		
		while(l->currentElem && strcmp(tmpCard->workshopName,s))
		{
			list_next(l);
			tmp = list_data(l);
			tmpCard = (Card*) tmp;
		}		
	}
	return tmp;
}



void list_print_char (list * l)
{
   	int i;
   	int size = l->nbElem;

   	list_first (l);
   	for (i = 0; i < size; i++)
	{
      	if (list_data (l) != NULL)
        	printf ("%s ", (char *) list_data (l));
      	
      	list_next (l);
      	printf ("\n");
   	}
}



void list_print_Card (list * l)
{
	int i;
   	int size = l->nbElem;
   	Card *tmpCard;

   	list_first (l);
   	
   	for (i = 0; i < size; i++)
	{
		tmpCard = (Card*) list_data (l);
      	if (list_data (l) != NULL)
      	{
      		printf("Card %d\t\t-->\t\t", i+1);
      		print_Cardstar(tmpCard);
      	}
      	list_next (l);
   	}
	printf("\n");
}


void list_print_Workshop (list * l)
{
	int i;
   	int size = l->nbElem;
   	Workshop *tmpWorkshop;

   	list_first (l);
   	
   	for (i = 0; i<size; i++)
	{
		tmpWorkshop = (Workshop*) list_data (l);
      	if (list_data (l) != NULL)
      	{
      		printf("Workshop %d\t\t-->\t\t", i+1);
      		print_Workshopstar(tmpWorkshop);
      	}
      	list_next (l);
   	}
	printf("\n");
}




