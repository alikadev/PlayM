#include <pm/sys.h>
#include <pm/sys/asserts.h>

#include <stdio.h>
#include <stdlib.h>

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
    ARG_ASSERT(list);

    if (list->next)
        linked_list_destroy(list->next);
    free(list);
}

void linked_list_destroy_purge(LinkedList *list)
{
    ARG_ASSERT(list);

    free(list->elem);
    
    if(list->next)
        linked_list_destroy_purge(list->next);
    free(list);
}

void linked_list_insert(LinkedList *list, void *data)
{
    ARG_ASSERT(list);

    LinkedList *new = linked_list_create(data);
    LinkedList *node = list;
    while (node->next)
        node = node->next;
    node->next = new;
}

void *linked_list_remove(LinkedList **pList, size_t item)
{
    ARG_ASSERT(pList);
    ARG_ASSERT(*pList);
    OUT_OF_BOUNCE_ASSERT(item, linked_list_size(*pList));

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
    ARG_ASSERT(list);

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
    ARG_ASSERT(list);
    OUT_OF_BOUNCE_ASSERT(item, linked_list_size(list));

    for (unsigned i = 0; i < item; i++)
        list = list->next;

    return list->elem;
}
