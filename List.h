#ifndef H_LISTESIMPLE
#define H_LISTESIMPLE

#include <stddef.h>             /* pour size_t */


typedef struct item
{
   struct item *next;
   void *data;
} item_s;

typedef struct st_list
{
   item_s *head;
   item_s *currentElem;
} list;

list *list_new (void);	// Create new list 
void list_insert (list *, void *);	// Insert a new element at the end of the list 
void list_removeNext (list *);	// Remove the next element of the list 
void list_removeFirst (list *);	// Remove the first element of the list
void list_next (list *);	// Access to the next element
void* list_data (list *);	// Access to the data 
void list_first (list *);	// Access to the firs element of the list 
void list_last (list *);	// Access to the last element of the list 
size_t listizeof (list *);	// Calculate the number of elements in the list
void list_delete (list **);	// Delete the list in the memory

void list_print_char (list *);
void list_print_Card (list *);

#endif /* not H_LISTESIMPLE */
