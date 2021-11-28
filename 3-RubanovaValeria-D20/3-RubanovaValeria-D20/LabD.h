#pragma once
#ifndef _MY_H 
#define _MY_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int compare(int * x, int* y);

int* readFile(const char* filename, int* B, int* N);

void writeFile(const char* filename, int* solution, int size);

bool** createMatrix(int c, int r);

void freeMatrix(bool** A, int c);

void fillMatrix(bool** A, int* set, int c, int r);

int* traceSubset(bool** A, int* set, int c, int r, int* subset_size);

int* subsetSum(int* set, int sum, int n, int* subset_size);

#ifdef __cplusplus
}
#endif

#endif
