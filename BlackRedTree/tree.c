#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

void CheckTree(rbtree_t tree)
{
  if (tree != NULL)
  {
    if (tree->right != NULL)
    {
      if (tree->right->prev != tree)
      {
        printf("Node: %i\n", tree->right->data);
      }
      CheckTree(tree->right);
    }
    if (tree->left != NULL)
    {
      if (tree->left->prev != tree)
      {
        printf("Node: %i\n", tree->left->data);
      }
      CheckTree(tree->left);
    }
  }
}

rbtree_t InitTree(void)
{
  return NULL;
}

void RightRot(rbtree_t *tree, rbtree_t a)
{
  rbtree_t b = a->prev;

  rbtree_t buf = a->right;
  a->right = b;
  b->left = buf;

  if (b->left != NULL)
    b->left->prev = b;

  if (b->prev != NULL)
  {
    if (b->prev->left == b)
      b->prev->left = a;
    else
      b->prev->right = a;
  }

  a->prev = b->prev;
  b->prev = a;

  if (*tree == b)
    *tree = a;
}

void LeftRot(rbtree_t *tree, rbtree_t a)
{
  rbtree_t b = a->prev;

  rbtree_t buf = a->left;
  a->left = b;
  b->right = buf;

  if (b->right != NULL)
    b->right->prev = b;

  if (b->prev != NULL)
  {
    if (b->prev->left == b)
      b->prev->left = a;
    else
      b->prev->right = a;
  }

  a->prev = b->prev;
  b->prev = a;

  if (*tree == b)
    *tree = a;
}

void Recolour(rbtree_t a)
{
  a->colour = 1 - a->colour;
  a->prev->colour = 1 - a->prev->colour;
  if (a == a->prev->left)
    a->prev->right->colour = 1 - a->prev->right->colour;
  else
    a->prev->left->colour = 1 - a->prev->left->colour;
}

void FixTreeAdd(rbtree_t *tree, rbtree_t son)
{
  if (son != NULL && son->prev != NULL && son->prev->prev != NULL)
  {
    rbtree_t dad = son->prev;
    rbtree_t grandpa = dad->prev;
    rbtree_t uncle = (grandpa->left == dad) ? grandpa->right : grandpa->left;

    if (uncle == grandpa->right)
    {
      //3 или 2 случай, нужен поворот с перекраской. Возможно, сначала левый поворот, потом правый, если 2 случай
      if (uncle == NULL || uncle->colour == BLACK)
      {
        if (son == dad->right)    //2 случай, 1 шаг
        {
          LeftRot(tree, son);
          son = dad;
          dad = son->prev;
        }
        RightRot(tree, dad);
        dad->colour = BLACK;
        grandpa->colour = RED;
      }
      else        //1 случай, перекраска и перебалансировка начиная с деда
      {
        Recolour(dad);
        FixTreeAdd(tree, grandpa);
      }
    }
    else
    {
      //3 или 2 случай, нужен поворот с перекраской. Возможно, сначала правый поворот, потом левый, если 2 случай
      if (uncle == NULL || uncle->colour == BLACK)
      {
        if (son == dad->left)    //2 случай, 1 шаг
        {
          RightRot(tree, son);
          son = dad;
          dad = son->prev;
        }
        LeftRot(tree, dad);
        dad->colour = BLACK;
        grandpa->colour = RED;
      }
      else        //1 случай, перекраска и перебалансировка начиная с деда
      {
        Recolour(dad);
        FixTreeAdd(tree, grandpa);
      }
    }
  }
}

void AddNode(rbtree_t *tree, int data)
{
  if (*tree == NULL)
  {
    *tree = malloc(sizeof(node_t));
    (*tree)->data = data;
    (*tree)->colour = BLACK;
    (*tree)->left = (*tree)->right = (*tree)->prev = NULL;
  }
  else
  {
    rbtree_t buf = *tree;
    rbtree_t son = malloc(sizeof(node_t));

    son->colour = RED;
    son->left = son->right = NULL;
    son->data = data;

    do
    {
      if (data < buf->data)
      {
        if (buf->left != NULL)
        {
          buf = buf->left;
        }
        else
        {
          buf->left = son;
          son->prev = buf;
        }
      }
      else
      {
        if (buf->right != NULL)
        {
          buf = buf->right;
        }
        else
        {
          buf->right = son;
          son->prev = buf;
        }
      }
    } while (buf->left != son && buf->right != son);

    if (buf->colour == RED)
    {
      FixTreeAdd(tree, son);
    }
    if ((*tree)->colour != BLACK)
      (*tree)->colour = BLACK;
  }
}

void FixTreeDelete(rbtree_t *tree, rbtree_t a, rbtree_t b)
{
  if (a != NULL && b != NULL)
  {
    if (a->right == b)
    {
      //1 случай
      if (b->colour == RED)
      {
        LeftRot(tree, b);
        b->colour = BLACK;
        a->colour = RED;
        b = a->right;
      }
      //2 случай
      if ((b->left == NULL || b->left->colour == BLACK) && (b->right == NULL || b->right->colour == BLACK))
      {
        b->colour = RED;
        if (a->colour == BLACK)
        {
          if (a->prev != NULL)
            FixTreeDelete(tree, a->prev, (a->prev->left == a) ? a->prev->right : a->prev->left);
        }
        else
        {
          a->colour = BLACK;
        }
        return;
      }
      else
      {
        //3 случай
        if ((b->left == NULL || b->left->colour == BLACK) && b->right != NULL && b->right->colour == BLACK)
        {
          b->left->colour = BLACK;
          b->colour = RED;
          RightRot(tree, b->left);
          b = b->prev;
        }
        //4 случай
        if (b->right != NULL && b->right->colour == RED)
        {
          b->colour = a->colour;
          b->right->colour = BLACK;
          a->colour = BLACK;
          LeftRot(tree, b);
        }
      }
    }
    else
    {
      if (a->left == b)
      {
        //1 случай
        if (b->colour == RED)
        {
          RightRot(tree, b);
          b->colour = BLACK;
          a->colour = RED;
          b = a->left;
        }
        //2 случай
        if ((b->right == NULL || b->right->colour == BLACK) && (b->left == NULL || b->left->colour == BLACK))
        {
          b->colour = RED;
          if (a->colour == BLACK)
          {
            if (a->prev != NULL)
              FixTreeDelete(tree, a->prev, (a->prev->right == a) ? a->prev->left : a->prev->right);
          }
          else
          {
            a->colour = BLACK;
          }
          return;
        }
        else
        {
          //3 случай
          if ((b->right == NULL || b->right->colour == BLACK) && b->left != NULL && b->left->colour == BLACK)
          {
            b->right->colour = BLACK;
            b->colour = RED;
            LeftRot(tree, b->right);
            b = b->prev;
          }
          //4 случай
          if (b->left != NULL && b->left->colour == RED)
          {
            b->colour = a->colour;
            b->left->colour = BLACK;
            a->colour = BLACK;
            RightRot(tree, b);
          }
        }
      }
    }
  }
}

void DeleteNode(rbtree_t *tree, int data)
{
  if (tree != NULL)
  {
    rbtree_t buf = *tree;
    rbtree_t a = NULL;
    rbtree_t b = NULL;

    while (buf->data != data)
    {
      if (data < buf->data)
      {
        if (buf->left == NULL) return;
        else buf = buf->left;
      }
      else
      {
        if (buf->right == NULL) return;
        else buf = buf->right;
      }
    }

    if (buf->left == NULL)
    {
      if (buf == *tree)
        *tree = buf->right;
      else
      {
        a = buf->prev;
        b = (buf->prev->left == buf) ? buf->prev->right : buf->prev->left;

        if (a->left == buf)
          a->left = buf->right;
        else
          a->right = buf->right;
      }
      if (buf->right != NULL)
        buf->right->prev = buf->prev;
    }
    else if (buf->right == NULL)
    {
      if (buf == *tree)
        *tree = buf->left;
      else
      {
        a = buf->prev;
        b = (buf->prev->left == buf) ? buf->prev->right : buf->prev->left;

        if (a->right == buf)
          a->right = buf->left;
        else
          a->left = buf->left;
      }
      buf->left->prev = buf->prev;
    }
    else
    {
      rbtree_t buf1 = buf->right;

      while (buf1->left != NULL)
        buf1 = buf1->left;

      buf->data = buf1->data;
      buf = buf1;

      a = buf->prev;
      b = (a->right == buf) ? a->left : a->right;

      if (a->left == buf)
        a->left = buf->right;
      else
        a->right = buf->right;

      if (buf->right != NULL)
        buf->right->prev = buf->prev;
    }

    if (buf->colour == BLACK && buf->prev != NULL)
    {
      FixTreeDelete(tree, a, b);
    }
    free(buf);
  }
}

void DeleteTree(rbtree_t tree)
{
  if (tree != NULL)
  {
    DeleteTree(tree->left);
    DeleteTree(tree->right);
    free(tree);
  }
}

int _countNodes(rbtree_t tree)
{
  if (tree != NULL)
    return _countNodes(tree->left) + _countNodes(tree->right) + 1;
  else
    return 0;
}

int _countHeight(rbtree_t tree, int height)
{
  if (tree->left != NULL)
  {
    if (tree->right != NULL)
    {
      int buf1 = _countHeight(tree->left, height + 1);
      int buf2 = _countHeight(tree->right, height + 1);
      return 1 + (buf1 < buf2 ? buf2 : buf1);
    }
    else
    {
      return 1 + _countHeight(tree->left, height + 1);
    }
  }
  else
  {
    if (tree->right != NULL)
    {

      return 1 + _countHeight(tree->right, height + 1);
    }
    else
    {
      return 1;
    }
  }
}

typedef struct elem
{
  int used;
  int data;
  enum colour colour;
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
        printf("%*i ", length - 2, matrix[i][j].data);
        if (matrix[i][j].colour == RED)
          printf("r");
        else
          printf("b");
      }
      else
      {
        for (k = 0; k < length; k++)
        {
          putchar(' ');
        }
      }
    }
    printf("\n\n");
  }
}

static void _matrixTree(rbtree_t tree, int y, int *x, matrix_t matrix)
{
  if (tree != NULL)
  {
    _matrixTree(tree->left, y + 1, x, matrix);
    matrix[y][*x].data = tree->data;
    matrix[y][*x].colour = tree->colour;
    matrix[y][*x].used = 1;
    (*x)++;
    _matrixTree(tree->right, y + 1, x, matrix);
  }
}

void PrintTree(rbtree_t tree)
{
  if (tree != NULL)
  {
    int x = _countNodes(tree);
    int y = _countHeight(tree, 0);
    matrix_t matrix = _createMatrix(x, y);
    int length = 4;
    int xbuf = 0;

    _matrixTree(tree, 0, &xbuf, matrix);
    _printMatrix(matrix, x, y, length);

    _deleteMatrix(matrix, y);
  }
}