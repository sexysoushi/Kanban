/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 */

#ifndef LIST_H
#define LIST_H

typedef struct item
{
   struct item *next;
   void *data;
} item_s;

typedef struct st_list
{
   item_s *head;
   item_s *currentElem;
   int nbElem;
} list;

list *list_new (void);	// Create new list 
void list_insert (list *, void *);	// Insert a new element at the end of the list 
void list_removeNext (list *);	// Remove the next element of the list 
void list_removeFirst (list *);	// Remove the first element of the list
void list_next (list *);	// Access to the next element
void* list_data (list *);	// Access to the data 
void list_first (list *);	// Access to the first element of the list 
void list_last (list *);	// Access to the last element of the list 
void list_delete (list **);	// Delete the list in the memory

char* list_seek_char(char*, list*);
void* list_seek_voidstar(char*, list*);

void list_print_char (list *);
void list_print_Card (list *);

#endif /* LIST_H */

