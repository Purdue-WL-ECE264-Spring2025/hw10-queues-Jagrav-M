#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) {return NULL; }

void insert_at_head(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
  if (!node) return;
  node->next = list->head;
  list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
    if (!node) return;
    if (list->head == NULL) 
    {
      list->head = node;
    } 
    else 
    {
      struct list_node *cur = list->head;
      while (cur->next != NULL) 
      {
        cur = cur->next;
      }
      cur->next = node;
    }
}

size_t remove_from_head(struct linked_list *list) 
{
  if (list->head == NULL) return 0;

    struct list_node *temp = list->head;
    size_t val = temp->value;
    list->head = temp->next;
    free(temp);
    return val;
}

size_t remove_from_tail(struct linked_list *list) 
{
  if (list->head == NULL) return 0;
  struct list_node *cur = list->head;
  if (cur->next == NULL) {
    size_t val = cur->value;
    free(cur);
    list->head = NULL;
    return val;
  }
  while (cur->next->next != NULL) 
  {
    cur = cur->next;
  }
  size_t val = cur->next->value;
  free(cur->next);
  cur->next = NULL;
  return val;
}

void free_list(struct linked_list list) 
{
  struct list_node *curr = list.head;
  while (curr != NULL) 
  {
    struct list_node *next = curr->next;
    free(curr);
    curr = next;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
