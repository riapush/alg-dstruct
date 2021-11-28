#include "LabD.h"
#pragma warning (disable:4996)

int compare(int * x, int* y) {
	return *x - *y;
}


int* readFile(const char* filename, int* B, int* N) {
	FILE* file = fopen(filename, "r");
	//check if opened;
	fscanf(file, "%i\n", B);
	fscanf(file, "%i\n", N);
	int* set = (int*)malloc((*N) * sizeof(int));
	for (int i = 0; i < *(N); i++) {
		fscanf(file, "%i ", (set + i));
	}
	fclose(file);
	return set;
}

void writeFile(const char* filename, int* solution, int size) {
	FILE* file = fopen(filename, "w");
	for (int i = size - 1; i >= 0; i--) {
		fprintf(file, "%i ", solution[i]);
	}
	fclose(file);
}

bool** createMatrix(int c, int r) { // c is length of column, r is length of row
	bool** A = (bool**)malloc(c * sizeof(bool*));
	if (A == NULL) {
		printf("Memory allocation error in createMatrix\n");
		return NULL;
	}
	for (int i = 0; i < c; i++) {
		A[i] = (bool*)malloc(r * sizeof(bool));
		if (A[i] == NULL) {
			for (int j = i - 1; j >= 0; j--)
				free(A[j]);
			free(A);
			printf("Memory allocation error for A[%i] in createMatrix\n", i);
			return NULL;
		}
	}
	return A;
}

void freeMatrix(bool** A, int c) {
	for (int i = 0; i < c; i++) {
		free(A[i]);
	}
	free(A);
}

void fillMatrix(bool** A, int* set, int c, int r) {
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			if (i == 0) {
				if (j == set[i] || j == 0) {
					A[i][j] = true;
				}
				else {
					A[i][j] = false;
				}
			}
			else {
				if ((A[i - 1][j] == true) || (A[i - 1][j - set[i]] == true)) {
					A[i][j] = true;
				}
				else {
					A[i][j] = false;
				}
			}
		}
	}

}

int* traceSubset(bool** A, int* set, int c, int r, int* subset_size) {
	(*subset_size) = 0;
	int* solution = NULL;
	int i = c - 1, j = r - 1;
	while (i >= 0) {
		if (i == 0 && A[i][j] == true) {
			(*subset_size)++;
			int* tmp = (int*)realloc(solution, *(subset_size) * sizeof(int));
			if (tmp == NULL) {
				printf("Memory allocation error in traceSubset\n");
				return NULL;
			}
			solution = tmp;
			solution[(*subset_size) - 1] = set[i];
			return solution;
		}
		if (A[i - 1][j] == true) {
			i--;
		}
		else {
			(*subset_size)++;
			int* tmp = (int*)realloc(solution, *(subset_size) * sizeof(int));
			if (tmp == NULL) {
				printf("Memory allocation error in traceSubset\n");
				return NULL;
			}
			solution = tmp;
			solution[(*subset_size) - 1] = set[i];
			printf("%d ", set[i]);
			j -= set[i];
			i--;
		}
	}
	return solution;
}

int* subsetSum(int* set, int sum, int n, int* subset_size) {
	int* solution = (int*)malloc(sizeof(int));
	if (solution == NULL) {
		printf("Memory allocation error in subsetSum\n");
		return NULL;
	}
	solution[0] = 0;
	bool** m = createMatrix(n, sum + 1);
	fillMatrix(m, set, n, sum + 1);
	if (m[n - 1][sum] == true) {
		free(solution);
		solution = traceSubset(m, set, n, sum + 1, subset_size);
	}
	freeMatrix(m, n);
	if (*subset_size == -1) {
		*subset_size = 1;
	}
	return solution;
}