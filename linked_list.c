#include "linked_list.h"

#include <stdlib.h>
//Create and return a new list node
struct list_node *new_node(size_t value) {
  struct list_node *node = malloc(sizeof(struct list_node));
  //if (!node) exit(1); // Exit on malloc failure
  node->value = value;
  node->next = NULL;
  return node;
 }
// Insert at the head of the list
void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
  node->next = list->head;
  list->head = node;
}

// Insert at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
  //if the list is empty, then the new node becomes the first and only node
  if (!list->head) {
    list->head = node;
    return;
  }

  struct list_node *cur = list->head;
  while (cur->next) {
    cur = cur->next;
  }
  cur->next = node;
}

// Remove and return the value from the head of the list
size_t remove_from_head(struct linked_list *list) { 
  if (!list->head) exit(1); // Nothing to remove

  struct list_node *node = list->head;
  size_t value = node->value;
  list->head = node->next;
  free(node);

  return value;
  }

size_t remove_from_tail(struct linked_list *list) {
  if (!list->head) exit(1);

  // if only 1 value
  if (list->head->next == NULL) {
    struct list_node *node = list->head;
    size_t value = node->value;
    list->head = node->next;
    free(node);
    return value;
  }
  // if there are multiple values
  struct list_node *cur = list->head;
  // step to the end
  while (cur->next) {
    if (cur -> next -> next == NULL){
      size_t value = cur -> next -> value;
      free(cur->next);
      cur -> next = NULL;
      return value;
    }
    cur = cur->next;
  }
  return 0;
}

void free_list(struct linked_list list) {
  struct list_node *cur = list.head;
  while (cur) {
    struct list_node *next = cur->next;
    free(cur);
    cur = next;
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
