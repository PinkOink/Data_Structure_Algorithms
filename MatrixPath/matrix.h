#ifndef MATRIX_H_INCLUDED__
#define MATRIX_H_INCLUDED__
#pragma once
#include <stdio.h>

typedef int **matrix_t;

matrix_t GetMatrix(FILE *f, int *n, int *m);

matrix_t CreateMatrix(int n, int m);

void FillMatrix(FILE *f, matrix_t mat, int n, int m);

void PrintMatrix(matrix_t mat, int n, int m);

void DeleteMatrix(matrix_t mat, int n);

matrix_t CountPathCost(matrix_t mat, int n, int m, int start);

int FindMinCost(matrix_t path, int n, int m, int start);

void PrintPath(matrix_t mat, matrix_t path, int i, int j, int start, int m);

#endif