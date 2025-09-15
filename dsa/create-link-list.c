#include <stdio.h>
#include <stdlib.h>

// Defining self referencing structure
typedef struct sNode
{
  int data;
  struct sNode *link;
} sNode;

int main()
{

  // creating front node
  sNode *front = NULL;
  sNode *first = NULL;
  sNode *second = NULL;

  // allocate memory for front node
  front = (sNode *)malloc(sizeof(sNode));
  front->data = 2;

  // allocate memory for first node
  first = (sNode *)malloc(sizeof(sNode));
  first->data = 20;

  // allocate memory for second node
  second = (sNode *)malloc(sizeof(sNode));
  second->data = 100;

  // linking part
  front->link = first;
  first->link = second;
  second->link = NULL;

  sNode *temp = front;

  printf("SingleLinked list: ");
  while (temp != NULL)
  {
    printf("%d -> ", temp->data);
    temp = temp->link;
  }
  printf("NULL\n");

  return 0;
}