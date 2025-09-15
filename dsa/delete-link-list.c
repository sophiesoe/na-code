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

  // deletion part
  sNode *temp = front;

  // before deletion
  printf("Original list: ");
  while (temp != NULL)
  {
    printf("%d -> ", temp->data);
    temp = temp->link;
  }
  printf("NULL\n");

  sNode *prev = NULL;
  temp = front;

  // key to search which to delete
  int key;
  printf("Enter the key you want to delete: ");
  scanf("%d", &key);

  if (temp != NULL && temp->data == key) // if it is the head to delete
  {
    front = temp->link;
    free(temp);
  }
  // if it is not the head to delete
  else
  {
    while (temp != NULL && temp->data != key)
    {
      prev = temp;
      temp = temp->link;
    }
    if (temp != NULL)
    {
      prev->link = temp->link;
      free(temp);
    }
  }

  // after deletion
  printf("After deletion %d : ", key);
  temp = front;
  while (temp != NULL)
  {
    printf("%d -> ", temp->data);
    temp = temp->link;
  }
  printf("NULL\n");

  return 0;
}