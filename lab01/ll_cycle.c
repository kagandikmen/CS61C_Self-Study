#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {

    // returns 0 for acyclic lists, and 1 for acyclic ones

    node *tortoise = head;
    node *hare = head;

    if (NULL == head)
    {
        // means that the list is empty
        // an empty list is also acyclic
        return 0;
    }
    
    do
    {
        hare = (hare->next);
        if (NULL == hare)
        {
            // means that the list is acyclic
            return 0;
        }

        hare = (hare->next);
        if (NULL == hare)
        {
            // means that the list is acyclic
            return 0;
        }
        else
        {
            tortoise = tortoise->next;
        }

    }while(tortoise != hare);

    // out of the do-while loop, means the list is cyclic then
    return 1;
}