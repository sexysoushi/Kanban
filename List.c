#include <stdlib.h>
#include "List.h"

list* list_new(void* v)
{
	list *l = (list*) malloc(sizeof(list));
	
	l->value = v;
	l->next = NULL;
	
	return l;
}

void list_add(list* l, void* v)
{
	list *e, *p;
	
	e = l;
	p = NULL;
	
	while(e != NULL)
	{
		p = e;
		e = e->next;
	}
	
	e = (list_elem*) malloc(sizeof(list_elem));
	e->value = v;
	e->next = NULL;
	
	if(p != NULL)
		p->next = e;
}

void list_del(list *l)
{
	list *e, *n;
	
	if(l == NULL)
		return;
	
	e = l;
	while(e!=NULL)
	{
		n = e->next;
		free(e);
		e = n;
	}
	
	l = NULL;
}



