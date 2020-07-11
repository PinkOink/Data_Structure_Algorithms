#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable: 4996)

int main(void)
{
  tree_t *tree;
  FILE *f = fopen("tree.txt", "r");

  tree = GetTree(f);
  PrintTree(tree);

  DeleteTree(tree);
  fclose(f);
  return 0;
}