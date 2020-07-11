//Написать процедуры добавления в бинарное дерево и удаления из него.
//Добавление выполнять в поддерево с меньшим чилом узлов.
//В узлах поддеревьев хранить разность числа узлов.
//Узлы дерева должны содержать указатели на предков.Рекурсию использовать запрещается.

#include "btree.h"
#include <stdio.h>
#pragma warning(disable: 4996)

int main(void)
{
  int in = -1;
  tree_t *tree = NULL;
  tree_t *buf;
  FILE *f;

  printf("Would you like to\n");
  printf("1 - get tree from file \"in.txt\"\n");
  printf("2 - initialize empty tree\n");
  printf("3 - exit");
  while (in != 1 && in != 2 && in != 3)
  {
    printf("\nEnter: ");
    scanf("%i", &in);
  }
  switch (in)
  {
  case 1:
    f = fopen("in.txt", "r");
    if (f != NULL)
      tree = GetTree(f);
    else
      return 0;
    fclose(f);
    break;
  case 2:
    tree = InitTree();
    break;
  case 3:
    return 0;
  default:
    break;
  }

  for (;;)
  {
    printf("\n");
    printf("Would you like to\n");
    printf("1 - find number\n");
    printf("2 - add number\n");
    printf("3 - delete number\n");
    printf("4 - print tree\n");
    printf("5 - exit\n");

    do
    {
      printf("\nEnter: ");
      scanf("%i", &in);
    } while (in < 1 || in > 5);

    switch (in)
    {
    case 1:
      printf("Enter the number to search: ");
      scanf("%i", &in);
      buf = FindData(tree, in);
      if (buf == NULL)
        printf("No such number in the tree\n");
      else
        printf("This number is in the tree\n");
      break;
    case 2:
      printf("Enter the number to add: ");
      scanf("%i", &in);
      AddNode(tree, in);
      printf("The number has been added\n");
      break;
    case 3:
      printf("Enter the number to delete: ");
      scanf("%i", &in);
      buf = DeleteNode(tree, in);
      if (buf == NULL)
        printf("No such number in the tree\n");
      else
        printf("This number has been deleted from the tree\n");
      break;
    case 4:
      printf("The tree:\n");
      PrintTree(tree);
      printf("\n");
      break;
    case 5:
      printf("You've chosen to exit the program.\n");
      DeleteTree(tree);
      return 0;
    default:
      break;
    }
  }

  return 0;
}