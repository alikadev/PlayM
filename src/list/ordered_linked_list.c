#include <pm/list.h>

#include <string.h> // strerror()
#include <errno.h> // errno
#include <stdlib.h> // malloc(), free()
#include <assert.h> // assert()

extern int errno;

OrderedLinkedList *ordered_linked_list_create(void *data, int (*compare)(void *, void *))
{
	OrderedLinkedList *list = malloc(sizeof *list);
	if(!list)
		assert(0 && strerror(errno));

	list->elem = data;
	list->next = NULL;
	list->compare = compare;
	return list;
}

void ordered_linked_list_destroy(OrderedLinkedList *list)
{
	if(!list)
		assert(0 && "Bad arg: ordered_linked_list_destroy 'list' argument is NULL!");
		
	if(list->next)
		ordered_linked_list_destroy(list->next);
	free(list);
}

void ordered_linked_list_insert(OrderedLinkedList **pList, void *data)
{
	if(!pList)
		assert(0 && "Bad arg: ordered_linked_list_insert 'pList' argument is NULL!");
	
	// Get the current root
	OrderedLinkedList *node = *pList;
	if(!node)
		assert(0 && "Bad arg: ordered_linked_list_insert 'pList' argument reference to NULL!");

	// Create the new node
	OrderedLinkedList *newNode = ordered_linked_list_create(data, node->compare);

	// Check if it fit in the first place
	int cmpFirst = node->compare(node->elem, newNode->elem);
	if (cmpFirst >= 0)
	{
		newNode->next = node;
		*pList = newNode;
		return;
	}
	// Ignore until the data fit (or the list ends)
	while (node->next != NULL)
	{
		int cmpNext = node->compare(node->next->elem, newNode->elem);
		if (cmpNext > 0) break;
		node = node->next;
	}
	// Insert the element
	newNode->next = node->next;
	node->next = newNode;
}

size_t ordered_linked_list_size(OrderedLinkedList *list)
{
	if(!list)
		assert(0 && "Bad arg: ordered_linked_list_insert 'list' argument is NULL!");

	size_t count = 1;
	while(list->next)
	{
		list = list->next;
		count++;
	}

	return count;
}

void *ordered_linked_list_get(OrderedLinkedList *list, size_t item)
{
	if(!list)
		assert(0 && "Bad arg: ordered_linked_list_insert 'list' argument is NULL!");

	if(ordered_linked_list_size(list) <= item)
		assert(0 && "Out of bounce check: ordered_linked_list_get 'item' is greater than list size");

	for (unsigned i = 0; i < item; i++)
		list = list->next;

	return list->elem;
}