#include <stdlib.h>
#include "matrix.h"
#pragma warning(disable: 4996)
#define lefter(a) ((a >= 0) ? (a - 1) : a)
#define righter(a, b) ((a < b) ? (a + 1) : a)

int min3(int x, int y, int z)
{
  if (x < y)
  {
    if (x < z)
      return x;
    else
      return z;
  }
  else
  {
    if (y < z)
      return y;
    else
      return z;
  }
}

matrix_t GetMatrix(FILE *f, int *n, int *m)
{
  matrix_t mat;

  fscanf(f, "%i", n);
  fscanf(f, "%i", m);

  mat = CreateMatrix(*n, *m);
  FillMatrix(f, mat, *n, *m);

  return mat;
}

matrix_t CreateMatrix(int n, int m)
{
  matrix_t mat = malloc(sizeof(int *) * n);
  int i;

  for (i = 0; i < n; i++)
  {
    mat[i] = malloc(sizeof(int) * m);
  }

  return mat;
}

void FillMatrix(FILE *f, matrix_t mat, int n, int m)
{
  int i, j;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < m; j++)
    {
      fscanf(f, "%i", &(mat[i][j]));
    }
  }
}

void PrintMatrix(matrix_t mat, int n, int m)
{
  int i, j;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < m; j++)
    {
      printf("%i ", mat[i][j]);
    }
    printf("\n");
  }
}

void DeleteMatrix(matrix_t mat, int n)
{
  int i;

  for (i = 0; i < n; i++)
  {
    free(mat[i]);
  }

  free(mat);
}

matrix_t CountPathCost(matrix_t mat, int n, int m, int start)
{
  matrix_t path = CreateMatrix(n, m);
  int i, j;
  int left = lefter(start);
  int right = righter(start, m);

  path[0][start] = mat[0][start];

  if (n > 1)
  {
    path[1][start] = path[0][start] + mat[1][start];
    if (left >= 0)
    {
      path[1][left] = path[0][start] + mat[1][left];
    }
    if (right < m)
    {
      path[1][right] = path[0][start] + mat[1][right];
    }
    left = lefter(left);
    right = righter(right, m);
  }

  for (i = 2; i < n; i++)
  {
    if (left >= 0)
    {
      path[i][left] = path[i - 1][righter(left, m)] + mat[i][left];
    }
    if (right < m)
    {
      path[i][right] = path[i - 1][lefter(right)] + mat[i][right];
    }

    path[i][left + 1] = min(path[i - 1][left + 1], path[i - 1][righter(left + 1, m)]) + mat[i][left + 1];
    path[i][right - 1] = min(path[i - 1][right - 1], path[i - 1][lefter(right - 1)]) + mat[i][right - 1];

    for (j = left + 2; j < right - 1; j++)
    {
      path[i][j] = min3(path[i - 1][j], path[i - 1][lefter(j)], path[i - 1][righter(j, m)]) + mat[i][j];
    }

    left = lefter(left);
    right = righter(right, m);
  }

  return path;
}

int FindMinCost(matrix_t path, int n, int m, int start)
{
  int min;
  int left, right;
  int i;

  left = (start - n + 1 < 0) ? 0 : (start - n + 1);
  right = (start + n - 1 < m) ? (start + n - 1) : (m - 1);

  min = left;

  for (i = left + 1; i <= right; i++)
  {
    min = (path[n - 1][min] < path[n - 1][i]) ? min : i;
  }

  return min;
}

void PrintPath(matrix_t mat, matrix_t path, int i, int j, int start, int m)
{
  if (i == 0)
  {
    printf("%i ", mat[i][j]);
  }
  else
  {
    int left = (start - i + 1 < 0) ? 0 : (start - i + 1);
    int right = (start + i - 1 < m) ? (start + i - 1) : (m - 1);

    if (j - 1 >= left)
    {
      if (j + 1 <= right)
      {
        if (path[i - 1][j - 1] < path[i - 1][j])
        {
          if (path[i - 1][j - 1] < path[i - 1][j + 1])
          {
            PrintPath(mat, path, i - 1, j - 1, start, m);
            printf("right\n");
          }
          else
          {
            PrintPath(mat, path, i - 1, j + 1, start, m);
            printf("left\n");
          }
        }
        else
        {
          if (path[i - 1][j] < path[i - 1][j + 1])
          {
            PrintPath(mat, path, i - 1, j, start, m);
            printf("down\n");
          }
          else
          {
            PrintPath(mat, path, i - 1, j + 1, start, m);
            printf("left\n");
          }
        }
      }
      else
      {
        if (j <= right)
        {
          if (path[i - 1][j] > path[i - 1][j - 1])
          {
            PrintPath(mat, path, i - 1, j - 1, start, m);
            printf("right\n");
          }
          else
          {
            PrintPath(mat, path, i - 1, j, start, m);
            printf("down\n");
          }
        }
        else
        {
          PrintPath(mat, path, i - 1, j - 1, start, m);
          printf("right\n");
        }
      }
    }
    else
    {
      if (j + 1 <= right)
      {
        if (j >= left)
        {
          if (path[i - 1][j] > path[i - 1][j + 1])
          {
            PrintPath(mat, path, i - 1, j + 1, start, m);
            printf("left\n");
          }
          else
          {
            PrintPath(mat, path, i - 1, j, start, m);
            printf("down\n");
          }
        }
        else
        {
          PrintPath(mat, path, i - 1, j + 1, start, m);
          printf("left\n");
        }
      }
      else
      {
        PrintPath(mat, path, i - 1, j, start, m);
        printf("down\n");
      }
    }

    printf("%i ", mat[i][j]);
  }
}