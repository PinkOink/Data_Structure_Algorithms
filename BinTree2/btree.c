#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable: 4996)

/*--------------------------------------Работа со стеком--------------------------------------------------*/
typedef struct list
{
  tree_t *node;
  struct list *next;
}list_t;

typedef list_t *stack_t;

static stack_t _initStack(void)
{
  stack_t buf = malloc(sizeof(list_t));

  buf->next = NULL;
  buf->node = NULL;

  return buf;
}

static void _addToStack(stack_t *stack, tree_t *node)
{
  stack_t buf = malloc(sizeof(list_t));
  buf->next = *stack; 
  buf->node = node;
  *stack = buf;
}

static tree_t *_takeFromStack(stack_t *stack)
{
  if ((*stack)->next == NULL)
  {
    free(*stack);
    return NULL;
  }
  else
  {
    stack_t buf = *stack;
    tree_t *node = buf->node;

    *stack = buf->next;
    free(buf);

    return node;
  }
}
/*--------------------------------------------------------------------------------------------------------*/

/*--------------------------------------Функции для добавления--------------------------------------------*/
tree_t *GetTree(FILE *f)
{
  if (f != NULL)
  {
    tree_t *tree = InitTree();
    int data;

    while (fscanf(f, "%i", &data))
    {
      AddNode(tree, data);
    }

    return tree;
  }
  else
  {
    return NULL;
  }
}

tree_t *InitTree(void)
{
  tree_t *buf = malloc(sizeof(tree_t));

  buf->prev = buf;
  buf->left = buf;
  buf->right = buf;

  return buf;
}

void AddNode(tree_t *tree, int data)
{
  if (tree->prev == tree && tree->left == tree && tree->right == tree)
  {
    tree->data = data;
    tree->number = 0;
    tree->prev = NULL;
    tree->right = NULL;
    tree->left = NULL;
  }
  else
  {
    tree_t *buf = tree;

    while (buf->right != NULL && buf->left != NULL)
    {
      if (buf->number > 0)
      {
        buf->number--;
        buf = buf->right;
      }
      else
      {
        buf->number++;
        buf = buf->left;
      }
    }

    if (buf->right == NULL)
    {
      buf->number--;
      buf->right = malloc(sizeof(tree_t));
      buf->right->prev = buf;
      buf = buf->right;
    }
    else
    {
      buf->number++;
      buf->left = malloc(sizeof(tree_t));
      buf->left->prev = buf;
      buf = buf->left;
    }
    buf->data = data;
    buf->number = 0;

    buf->right = NULL;
    buf->left = NULL;
  }
}
/*--------------------------------------------------------------------------------------------------------*/

/*--------------------------------------Функции для удаления----------------------------------------------*/
tree_t *FindData(tree_t *tree, int data)
{
  stack_t stack = _initStack();
  tree_t *buf;

  _addToStack(&stack, tree);
  while ((buf = _takeFromStack(&stack)) != NULL)
  {
    if (buf->left != NULL)
    {
      _addToStack(&stack, buf->left);
    }
    if (buf->right != NULL)
    {
      _addToStack(&stack, buf->right);
    }
    if (buf->data == data)
    {
      while (_takeFromStack(&stack));

      return buf;
    }
  }

  return NULL;
}

static tree_t *_findLowestNode(tree_t *tree)
{
  tree_t *buf = tree;

  while (buf->left != NULL || buf->right != NULL)
  {
    if (buf->number > 0)
    {
      buf = buf->left;
    }
    else
    {
      buf = buf->right;
    }
  }

  return buf;
}

tree_t *DeleteNode(tree_t *tree, int data)
{
  if (tree->prev != tree && tree->left != tree && tree->right != tree)
  {
    tree_t *node = FindData(tree, data);

    if (node != NULL)
    {
      tree_t *low = _findLowestNode(tree);

      if (low != node)
      {
        int buf = node->data;
        node->data = low->data;
        low->data = buf;
      }

      node = low->prev;
      if (node != NULL)
      {
        if (node->left == low)
        {
          node->left = NULL;
          node->number--;
        }
        else
        {
          node->right = NULL;
          node->number++;
        }

        free(low);

        low = node;
        node = low->prev;

        while (node != NULL)
        {
          if (node->left == low)
          {
            node->number--;
          }
          else
          {
            node->number++;
          }
          low = node;
          node = low->prev;
        }
      }
      else
      {
        low->prev = low;
        low->right = low;
        low->left = low;
      }

      return tree;
    }
    else
    {
      return NULL;
    }
  }
  return NULL;
}

void DeleteTree(tree_t *tree)
{
  if (tree->prev == tree && tree->left == tree && tree->right == tree)
  {
    free(tree);
  }
  else
  {
    stack_t stack = _initStack();
    tree_t *buf;

    _addToStack(&stack, tree);
    while ((buf = _takeFromStack(&stack)) != NULL)
    {
      if (buf->left != NULL)
      {
        _addToStack(&stack, buf->left);
      }
      if (buf->right != NULL)
      {
        _addToStack(&stack, buf->right);
      }
      free(buf);
    }
  }
}
/*--------------------------------------------------------------------------------------------------------*/

/*--------------------------------------Функции для печати------------------------------------------------*/
static int _countHeight(tree_t *tree)
{
  tree_t *buf = tree;
  int n = 1;

  while (buf->left != NULL || buf->right != NULL)
  {
    if (buf->number > 0)
    {
      buf = buf->left;
    }
    else
    {
      buf = buf->right;
    }
    n++;
  }

  return n;
}

static int _countNodes(tree_t *tree)
{
  stack_t stack = _initStack();
  tree_t *buf;
  int n = 1;

  _addToStack(&stack, tree);
  while ((buf = _takeFromStack(&stack)) != NULL)
  {
    if (buf->left != NULL)
    {
      _addToStack(&stack, buf->left);
    }
    if (buf->right != NULL)
    {
      _addToStack(&stack, buf->right);
    }
    n++;
  }

  return n;
}

static int _findLongestNumber(tree_t *tree)
{
  stack_t stack = _initStack();
  tree_t *buf;
  int max1 = abs(tree->data);
  int i; 

  _addToStack(&stack, tree);
  while ((buf = _takeFromStack(&stack)) != NULL)
  {
    if (buf->left != NULL)
    {
      _addToStack(&stack, buf->left);
    }
    if (buf->right != NULL)
    {
      _addToStack(&stack, buf->right);
    }
    if (abs(buf->data) > max1)
    {
      max1 = abs(buf->data);
    }
  }

  for (i = 0; max1 > 0 ; i++)
  {
    max1 /= 10;
  }

  return i;
}

typedef struct elem
{
  int used;
  int data;
  int number;
}elem_t;

typedef elem_t **matrix_t;

static matrix_t _createMatrix(int x, int y)
{
  matrix_t matrix = malloc(y * sizeof(elem_t *));
  int i, j;

  for (i = 0; i < y; i++)
  {
    matrix[i] = malloc(x * sizeof(elem_t));

    for (j = 0; j < x; j++)
    {
      matrix[i][j].used = 0;
    }
  }

  return matrix;
}

static void _deleteMatrix(matrix_t matrix, int y)
{
  int i;

  for (i = 0; i < y; i++)
  {
    free(matrix[i]);
  }

  free(matrix);
}

static void _printMatrix(matrix_t matrix, int x, int y, int length)
{
  int i, j, k;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < x; j++)
    {
      if (matrix[i][j].used == 1)
      {
        printf("%*i", length, matrix[i][j].data);
      }
      else
      {
        for (k = 0; k < length; k++)
        {
          putchar(' ');
        }
      }
    }
    printf("\n");
  }
}

static void _matrixTree(tree_t *tree, int y, int *x, matrix_t matrix)
{
  if (tree != NULL)
  {
    _matrixTree(tree->left, y + 1, x, matrix);
    matrix[y][*x].data = tree->data;
    matrix[y][*x].number = tree->number;
    matrix[y][*x].used = 1;
    (*x)++;
    _matrixTree(tree->right, y + 1, x, matrix);
  }
}

void PrintTree(tree_t *tree)
{
  if (tree->prev != tree && tree->left != tree && tree->right != tree && tree != NULL)
  {
    int x = _countNodes(tree);
    int y = _countHeight(tree);
    matrix_t matrix = _createMatrix(x, y);
    int length = _findLongestNumber(tree) + 1;
    int xbuf = 0;

    _matrixTree(tree, 0, &xbuf, matrix);
    _printMatrix(matrix, x, y, length);

    _deleteMatrix(matrix, y);
  }
}
/*--------------------------------------------------------------------------------------------------------*/