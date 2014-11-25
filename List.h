#ifndef LIST_H
#define LIST_H

typedef struct list_element
{
	void* value;
    struct list_element* next;
} list_elem;

typedef list_elem list;

list* list_new(void*); /* Create new list */
void list_del(list*);	/* Delete list */
void list_add(list*, void*);	/* Add one element at the end of the list */
void list_take_all_elem();	/* Take all the list's elements*/


#endif /* LIST_H */
