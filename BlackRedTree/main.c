#include "tree.h"
#include <stdio.h>

int main(void)
{
  rbtree_t tree = InitTree();
  int i;
  for (i = 0; i < 8; ++i)
  {
    AddNode(&tree, i);
    CheckTree(tree);
  }

  PrintTree(tree);
  printf("\n");


    printf("\t#%i\n", i);
    DeleteNode(&tree, 7);
    DeleteNode(&tree, 3);
    PrintTree(tree);
    printf("\n");
    CheckTree(tree);

  DeleteTree(tree);

  return 0;
}