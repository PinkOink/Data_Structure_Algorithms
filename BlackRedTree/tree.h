#ifndef TREE_H_INCLUDED__
#define TREE_H_INCLUDED__
#pragma once

enum colour
{
  RED, BLACK
};

typedef struct node_t
{
  enum colour colour;
  struct node_t *prev;
  struct node_t *right;
  struct node_t *left;
  int data;
}node_t;

typedef node_t *rbtree_t;

void CheckTree(rbtree_t tree);

rbtree_t InitTree(void);

void AddNode(rbtree_t *tree, int data);

void DeleteNode(rbtree_t *tree, int data);

void DeleteTree(rbtree_t tree);

void PrintTree(rbtree_t tree);

#endif