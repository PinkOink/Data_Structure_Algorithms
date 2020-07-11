#ifndef BTREE_H_INCLUDED__
#define BTREE_H_INCLUDED__
#pragma once
#include <stdio.h>

typedef struct tree
{
  int data;
  int number;
  struct tree *right;
  struct tree *left;
  struct tree *prev;
}tree_t;

tree_t *InitTree(void);

tree_t *GetTree(FILE *f);

void AddNode(tree_t *tree, int data);

tree_t *FindData(tree_t *tree, int data);

tree_t *DeleteNode(tree_t *tree, int data);

void PrintTree(tree_t *tree);

void DeleteTree(tree_t *tree);

#endif