#pragma warning(disable: 4996)
#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUF 20

char *_getWord(FILE *f)
{
  char buf[MAX_BUF];
  char *word;
  int i = 0;

  while ((buf[i] = fgetc(f)) != ' ' && buf[i] != EOF && i < MAX_BUF - 1)
  {
    i++;
  }

  if (buf[0] == ' ')
    fgetc(f);

  buf[i] = 0;

  if (i > 0)
  {
    word = malloc((i + 1) * sizeof(char));
    strcpy(word, buf);
    return word;
  }
  else
  {
    return NULL;
  }
}

tree_t *GetTree(FILE *f)
{
  char *word = _getWord(f);

  if (word == NULL)
  {
    return NULL;
  }
  else
  {
    tree_t *node = malloc(sizeof(tree_t));
    node->word = word;
    node->left = GetTree(f);
    node->right = GetTree(f);
    return node;
  }
}

void DeleteTree(tree_t *tree)
{
  if (tree == NULL)
  {
    return;
  }
  else
  {
    free(tree->word);
    DeleteTree(tree->left);
    DeleteTree(tree->right);
    free(tree);
  }
}

static int _countHeight(tree_t *tree, int n)
{
  if (tree == NULL)
  {
    return n;
  }
  else
  {
    if (tree->left != NULL || tree->right != NULL)
    {
      int hLeft = _countHeight(tree->left, n + 1);
      int hRight = _countHeight(tree->right, n + 1);

      return (hLeft < hRight) ? (hRight) : (hLeft);
    }
    else
    {
      return n + 1;
    }
  }
}

static int _countNodes(tree_t *tree)
{
  if (tree == NULL)
  {
    return 0;
  }
  else
  {
    return 1 + _countNodes(tree->left) + _countNodes(tree->right);
  }
}

static int _findLongestWord(tree_t *tree)
{
  if (tree == NULL)
  {
    return 0;
  }
  else
  {
    int maxLeft = _findLongestWord(tree->left);
    int maxRight = _findLongestWord(tree->right);
    int length = strlen(tree->word);

    if (length > maxRight)
    {
      if (length > maxLeft)
      {
        return length;
      }
      else
      {
        return maxLeft;
      }
    }
    else
    {
      if (maxRight > maxLeft)
      {
        return maxRight;
      }
      else
      {
        return maxLeft;
      }
    }
  }
}

typedef char **matrix_t;

static matrix_t *_createMatrix(int x, int y)
{
  matrix_t *matrix = malloc(y * sizeof(matrix_t));
  int i, j;

  for (i = 0; i < y; i++)
  {
    matrix[i] = malloc(x * sizeof(char *));

    for (j = 0; j < x; j++)
    {
      matrix[i][j] = NULL;
    }
  }

  return matrix;
}

static void _deleteMatrix(matrix_t *matrix, int y)
{
  int i;

  for (i = 0; i < y; i++)
  {
    free(matrix[i]);
  }

  free(matrix);
}

static void _printMatrix(matrix_t *matrix, int x, int y, int length)
{
  int i, j, k;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < x; j++)
    {
      if (matrix[i][j] != NULL)
      {
        printf("%*s", length, matrix[i][j]);
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

static void _matrixTree(tree_t *tree, int y, int *x, matrix_t *matrix)
{
  if (tree != NULL)
  {
    _matrixTree(tree->left, y + 1, x, matrix);
    matrix[y][*x] = tree->word;
    (*x)++;
    _matrixTree(tree->right, y + 1, x, matrix);
  }
}

void PrintTree(tree_t *tree)
{
  int x = _countNodes(tree);
  int y = _countHeight(tree, 0);
  matrix_t *matrix = _createMatrix(x, y);
  int length = _findLongestWord(tree);
  int xbuf = 0;

  _matrixTree(tree, 0, &xbuf, matrix);
  _printMatrix(matrix, x, y, length);

  _deleteMatrix(matrix, y);
}
