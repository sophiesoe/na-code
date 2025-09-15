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
  // creating nodes
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

  // before inserting
  printf("Original list: ");
  sNode *temp = front;
  while (temp != NULL)
  {
    printf("%d -> ", temp->data);
    temp = temp->link;
  }
  printf("NULL\n");

  // inserting with position
  int position;
  int addData;

  printf("Enter the data you want to insert: ");
  scanf("%d", &addData);

  printf("Enter the position you want to insert: ");
  scanf("%d", &position);

  // create newNode
  sNode *newNode = (sNode *)malloc(sizeof(sNode));
  newNode->data = addData;
  newNode->link = NULL;

  if (position == 1)
  {
    // inserting at the beginning
    newNode->link = front;
    front = newNode;
  }
  // inserting in between
  else
  {
    temp = front;
    for (int i = 1; i < position - 1 && temp != NULL; i++)
    {
      temp = temp->link;
    }
    // if the position is invalid
    if (temp == NULL)
    {
      printf("Position out of range!\n");
    }
    else
    {
      newNode->link = temp->link;
      temp->link = newNode;
    }
  }

  // after insertion
  printf("After inserting %d at position %d: ", addData, position);
  temp = front;
  while (temp != NULL)
  {
    printf("%d -> ", temp->data);
    temp = temp->link;
  }
  printf("NULL\n");

  return 0;
}
