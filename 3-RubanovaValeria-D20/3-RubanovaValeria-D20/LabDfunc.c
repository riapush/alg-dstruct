#include "LabD.h"
#pragma warning (disable:4996)


int compare(int* x, int* y) {
	return *x - *y;
}

int* readFile(const char* filename, long int* b, int* n) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Unable to open file\n");
		return NULL;
	}
	fscanf(file, "%li\n", b);
	fscanf(file, "%i\n", n);
	int* set = (int*)malloc((*n) * sizeof(int));
	for (int i = 0; i < *(n); i++) {
		fscanf(file, "%i ", (set + i));
	}
	fclose(file);
	return set;
}

int writeFile(const char* filename, int* solution, int size) {
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Unable to open file\n");
		return -1;
	}
	for (int i = size - 1; i >= 0; i--) {
		fprintf(file, "%i ", solution[i]);
	}
	fclose(file);
	return 0;
}

bool** createMatrix(int c, long int r) { // c is length of column, r is length of row
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

void freeMatrix(bool** matrix, int c) {
	for (int i = 0; i < c; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void fillMatrix(bool** matrix, int* set, int c, long int r) {
	for (long int i = 0; i < c; i++) {
		for (long int j = 0; j < r; j++) {
			if (i == 0) {
				if (j == set[i] || j == 0) {
					matrix[i][j] = true;
				}
				else {
					matrix[i][j] = false;
				}
			}
			else {
				if ((matrix[i - 1][j] == true) || (matrix[i - 1][j - set[i]] == true)) {
					matrix[i][j] = true;
				}
				else {
					matrix[i][j] = false;
				}
			}
		}
	}

}

int* traceSubset(bool** matrix, int* set, int c, long int r, int* subset_size) {
	(*subset_size) = 0;
	int* solution = NULL;
	int i = c - 1;
	long int j = r - 1;
	while (i >= 0) {
		if (i == 0 && matrix[i][j] == true) {
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
		if (matrix[i - 1][j] == true) {
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
			j -= set[i];
			i--;
		}
	}
	return solution;
}

int* subsetSum(int* set, long int sum, int n, int* subset_size) {
	int* solution = (int*)malloc(sizeof(int));
	if (solution == NULL) {
		printf("Memory allocation error in subsetSum\n");
		return NULL;
	}
	solution[0] = 0;
	bool** m = createMatrix(n, sum + 1);
	if (m == NULL) {
		printf("Memory allocation error in subsetSum\n");
		free(solution);
		return NULL;
	}
	fillMatrix(m, set, n, sum + 1);
	if (m[n - 1][sum] == true) {
		free(solution);
		solution = traceSubset(m, set, n, sum + 1, subset_size);
		if (solution == NULL) {
			printf("Memory allocation error in subsetSum\n");
			freeMatrix(m, n);
			return NULL;
		}
	}
	freeMatrix(m, n);
	if (*subset_size == -1) {
		*subset_size = 1;
	}
	return solution;
}

int labSolution(const char* input, const char* output) {
	int* a = NULL;
	long int b = -1;
	int n = -1;
	int subset_size = -1;
	a = readFile(input, &b, &n);
	if (a == NULL) {
		printf("Memory allocation error in readFile\n");
		return -1;
	}
	qsort(a, n, sizeof(int), (int(*) (const void *, const void *))compare);
	int* solution = subsetSum(a, b, n, &subset_size);
	if (solution == NULL) {
		free(a);
		return -1;
	}
	int check = writeFile(output, solution, subset_size);
	if (check == -1) {
		free(a);
		free(solution);
		return -1;
	}
	free(a);
	free(solution);
	return 0;
}

