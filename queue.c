#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/*
 * Declaring a helper functions here given to the fact that queue.h is not
 * allowed to be changed. Allocate new node and let pointer to pointer to
 * element_t point to newly allocated address of element_t with char array
 * initilized.
 */
bool __q_ele_new(element_t **pptr_element, char *s);

/*
 * Declaring a helper functions here given to the fact that queue.h is not
 * allowed to be changed. Allocate new node and let pointer to pointer to
 * element_t point to newly allocated address of element_t with char array
 * initilized.
 */
element_t *__q_remove(struct list_head *head, char *sp, size_t bufsize);

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *l = malloc(sizeof(struct list_head));
    if (l == NULL)
        return NULL;
    INIT_LIST_HEAD(l);
    return l;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l || list_empty(l)) {
        free(l);
        return;
    }
    element_t *element, *n;
    list_for_each_entry_safe (element, n, l, list) {
        q_release_element(element);
    }
    free(l);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *element = NULL;
    if (!__q_ele_new(&element, s)) {
        return false;
    }
    list_add(&(element->list), head);
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *element = NULL;
    if (!__q_ele_new(&element, s)) {
        return false;
    }
    list_add_tail(&(element->list), head);
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return __q_remove(head->next, sp, bufsize);
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return __q_remove(head->prev, sp, bufsize);
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return NULL if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *prev_node = head;
    struct list_head *next_node;
    struct list_head *curr_node = prev_node->next;

    prev_node->prev = prev_node;
    prev_node->next = prev_node;

    while (curr_node != head) {
        next_node = curr_node->next;
        curr_node->next = prev_node;
        prev_node->prev = curr_node;

        head->next = curr_node;
        curr_node->prev = head;
        prev_node = curr_node;
        curr_node = next_node;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head) {}

/*
 * Self-defined function: Allocate new node and let pointer to pointer to
 * element_t point to newly allocated address of element_t with char array
 * initilized.
 */
bool __q_ele_new(element_t **pptr_element, char *s)
{
    *pptr_element = malloc(sizeof(element_t));
    if (*pptr_element == NULL) {
        free(*pptr_element);
        return false;
    }
    (*pptr_element)->value = malloc(sizeof(char) * (strlen(s) + 1));
    if ((*pptr_element)->value == NULL) {
        return false;
    }
    // Initilize list_head
    INIT_LIST_HEAD(&(*pptr_element)->list);
    return true;
}

/*
 * Self-defined function to generailize q_remove_tail and q_remove_head
 */
element_t *__q_remove(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;
    list_del_init(head);
    // cppcheck-suppress nullPointer
    element_t *element = list_entry(head, element_t, list);
    if (sp != NULL) {
        strncpy(sp, element->value, (bufsize - 1));
        sp[bufsize - 1] = '\0';
    }
    return element;
}