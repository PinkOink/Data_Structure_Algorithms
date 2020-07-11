#ifndef BTREE_H_INCLUDED__
#define BTREE_H_INCLUDED__
#pragma once

#include <stdio.h>

typedef struct tree
{
  char *word;
  struct tree *left;
  struct tree *right;
}tree_t;

tree_t *GetTree(FILE *f);

void DeleteTree(tree_t *tree);

void PrintTree(tree_t *tree);

#endif