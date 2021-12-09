#ifndef _MY_H 
#define _MY_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int compare(int * x, int* y);

int* readFile(const char* filename, long int* b, int* n);

int writeFile(const char* filename, int* solution, int size);

bool** createMatrix(int c, long int r);

void freeMatrix(bool** matrix, int c);

void fillMatrix(bool** matrix, int* set, int c, long int r);

int* traceSubset(bool** matrix, int* set, int c, long int r, int* subset_size);

int* subsetSum(int* set, long int sum, int n, int* subset_size);

int labSolution(const char* input, const char* output);

#ifdef __cplusplus
}
#endif

#endif

