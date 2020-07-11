#include <stdio.h>
#include "matrix.h"
#pragma warning(disable: 4996)

int main(void)
{
  matrix_t mat, path;
  FILE *f = fopen("in.txt", "r");
  int n, m;
  int start = 3;

  mat = GetMatrix(f, &n, &m);
  PrintMatrix(mat, n, m);

  path = CountPathCost(mat, n, m, start);
  PrintMatrix(path, n, m);
//  printf("%i\n", FindMinCost(path, n, m, start));

  PrintPath(mat, path, n - 1, FindMinCost(path, n, m, start), start, m);

  DeleteMatrix(mat, n);
  DeleteMatrix(path, n);
  fclose(f);

  return 0;
}