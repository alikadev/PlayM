#include <pm/sys.h>
#include <pm/sys/asserts.h>

#include <string.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

OrderedLinkedList *ordered_linked_list_create(void *data, int (*compare)(void *, void *))
{
    ARG_ASSERT(compare);

    OrderedLinkedList *list = malloc(sizeof *list);
    ALLOC_ASSERT(list);

    list->elem = data;
    list->next = NULL;
    list->compare = compare;
    return list;
}

void ordered_linked_list_destroy(OrderedLinkedList *list)
{
    ARG_ASSERT(list);
        
    if(list->next)
        ordered_linked_list_destroy(list->next);
    free(list);
}

void ordered_linked_list_destroy_purge(OrderedLinkedList *list)
{
    ARG_ASSERT(list);

    free(list->elem);

    if(list->next)
        ordered_linked_list_destroy_purge(list->next);
    free(list);
}

void ordered_linked_list_insert(OrderedLinkedList **pList, void *data)
{
    ARG_ASSERT(pList);
    ARG_ASSERT(*pList);
    
    OrderedLinkedList *node = *pList;
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

void *ordered_linked_list_remove(OrderedLinkedList **pList, size_t item)
{
    ARG_ASSERT(pList);
    ARG_ASSERT(*pList);
    OUT_OF_BOUNCE_ASSERT(item, ordered_linked_list_size(*pList));

    // Get the node to destroy and unlinked it
    OrderedLinkedList *nodeToFree;
    if(item == 0)
    {
        nodeToFree = *pList;
        *pList = nodeToFree->next;
    } else {
        OrderedLinkedList *node = *pList;
        for (unsigned i = 0; i < item - 1; i++)
            node = node->next;

        nodeToFree = node->next;
        node->next = nodeToFree->next;
    }

    void *elem = nodeToFree->elem;
    nodeToFree->next = NULL;
    ordered_linked_list_destroy(nodeToFree);

    return elem;
}

size_t ordered_linked_list_size(OrderedLinkedList *list)
{
    ARG_ASSERT(list);

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
    ARG_ASSERT(list);
    OUT_OF_BOUNCE_ASSERT(item, ordered_linked_list_size(list));

    for (unsigned i = 0; i < item; i++)
        list = list->next;

    return list->elem;
}

