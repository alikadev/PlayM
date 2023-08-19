#ifndef PM_LIST_H
#define PM_LIST_H

#include <stddef.h> // size_t

typedef struct LinkedList LinkedList;
struct LinkedList
{
	LinkedList *next;
	void *elem;
};

LinkedList *linked_list_create(void *data);
void        linked_list_destroy(LinkedList *list);
void        linked_list_insert(LinkedList *list, void *data);
size_t      linked_list_size(LinkedList *list);
void *      linked_list_get(LinkedList *list, size_t item);

typedef struct OrderedLinkedList OrderedLinkedList;
struct OrderedLinkedList
{
	OrderedLinkedList *next;
	void *elem;
	int (*compare)(void *, void *);
};

OrderedLinkedList * ordered_linked_list_create(void *data, int (*compare)(void *, void *));
void                ordered_linked_list_destroy(OrderedLinkedList *list);
void                ordered_linked_list_insert(OrderedLinkedList **pList, void *data);
size_t              ordered_linked_list_size(OrderedLinkedList *list);
void *              ordered_linked_list_get(OrderedLinkedList *list, size_t item);

#endif