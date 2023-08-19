#include <pm/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

LinkedList *linked_list_create(void *data)
{
	LinkedList *list = malloc(sizeof *list);
	if(!list)
	{
		assert(0 && "Fail to create the linked list...\n");
	}

	list->next = NULL;
	list->elem = data;
	return list;
}

void linked_list_destroy(LinkedList *list)
{
	if(!list)
		assert(0 && "Bad arg: linked_list_destroy 'list' argument is NULL!");

	if (list->next)
		linked_list_destroy(list->next);
	free(list);
}

void linked_list_insert(LinkedList *list, void *data)
{
	if(!list)
		assert(0 && "Bad arg: linked_list_insert 'list' argument is NULL!");

	LinkedList *new = linked_list_create(data);
	LinkedList *node = list;
	while (node->next)
		node = node->next;
	node->next = new;

	for (size_t i = 0; i < linked_list_size(list); i++)
	{
		printf("(%s)", (char*)linked_list_get(list, i));
	}
	printf("\n");
}

size_t linked_list_size(LinkedList *list)
{
	if(!list)
		assert(0 && "Bad arg: linked_list_size 'list' argument is NULL!");

	size_t count = 1;
	LinkedList *node = list;

	while(node->next)
	{
		count++;
		node = node->next;
	}

	return count;
}

void *linked_list_get(LinkedList *list, size_t item)
{
	if(!list)
		assert(0 && "Bad arg: linked_list_get 'list' argument is NULL!");

	if(linked_list_size(list) <= item)
		assert(0 && "Out of bounce check: linked_list_get 'item' is greater than list size");

	for (unsigned i = 0; i < item; i++)
		list = list->next;

	return list->elem;
}
