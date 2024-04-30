#include <pm/sys.h>
#include <pm/debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

LinkedList *linked_list_create(void *data)
{
    debugfn();
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
    debugfn();
    if(!list)
        assert(0 && "Bad arg: linked_list_destroy 'list' argument is NULL!");

    if (list->next)
        linked_list_destroy(list->next);
    free(list);
}

void linked_list_destroy_purge(LinkedList *list)
{
    debugfn();
    if(!list)
        assert(0 && "Bad arg: linked_list_destroy_purge 'list' argument is NULL!");

    free(list->elem);
    
    if(list->next)
        linked_list_destroy_purge(list->next);
    free(list);
}

void linked_list_insert(LinkedList *list, void *data)
{
    debugfn();
    if(!list)
        assert(0 && "Bad arg: linked_list_insert 'list' argument is NULL!");

    LinkedList *new = linked_list_create(data);
    LinkedList *node = list;
    while (node->next)
        node = node->next;
    node->next = new;
}

void *linked_list_remove(LinkedList **pList, size_t item)
{
    debugfn();
    if(!pList)
        assert(0 && "Bad arg: linked_list_remove 'pList' argument is NULL!");

    if(!*pList)
        assert(0 && "Bad arg: linked_list_remove 'pList' argument point to NULL!");

    if(linked_list_size(*pList) <= item)
        assert(0 && "Out of bounce check: linked_list_remove 'item' is greater than pList size");

    // Get the node to destroy and unlinked it
    LinkedList *nodeToFree = NULL;
    if(item == 0)
    {
        nodeToFree = *pList;
        *pList = (*pList)->next;
    } else {
        LinkedList *node = *pList;
        for (unsigned i = 0; i < item - 1; i++)
            node = node->next;

        nodeToFree = node->next;
        node->next = nodeToFree->next;
    }

    void *elem = nodeToFree->elem;
    nodeToFree->next = NULL;
    linked_list_destroy(nodeToFree);

    return elem;
}

size_t linked_list_size(LinkedList *list)
{
    debugfn();
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
    debugfn();
    if(!list)
        assert(0 && "Bad arg: linked_list_get 'list' argument is NULL!");

    if(linked_list_size(list) <= item)
        assert(0 && "Out of bounce check: linked_list_get 'item' is greater than list size");

    for (unsigned i = 0; i < item; i++)
        list = list->next;

    return list->elem;
}
