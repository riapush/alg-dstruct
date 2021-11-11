#include <stdio.h>
#include <stdlib.h>
#include "LabC.h"
#pragma warning(disable : 4996)

int main(void) {
	fscanf(stdin, "%d", &n);

	tree = (int**)malloc(sizeof(int*) * n);
	if (tree == NULL) {
		printf("Memory allocation error for tree in main\n");
		return 1;
	}

	for (int i = 0; i < n; i++) {
		tree[i] = (int*)malloc(sizeof(int) * n);
		if (tree[i] == NULL) {
			for (int j = i - 1; j >= 0; j--)
				free(tree[j]);
			printf("Memory allocation error for tree[%i] in main\n", i);
			return 1;
		}
	}
	visited = (int*)malloc(sizeof(int) * n);
	if (visited == NULL) {
		for (int i = 0; i < n; i++)
			free(tree[i]);
		free(tree);
		printf("Memore allocation error for visited in main\n");
		return 1;
	}

	for (int i = 0; i < n; i++)
		visited[i] = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			tree[i][j] = -1;
	}

	int x, y;
	while (fscanf(stdin, "%d %d", &x, &y) != EOF) {
		tree[x][y] = 1;
		tree[y][x] = 1;
	}

	if (breadthFirstSearch(0) == -1)
		printf("Error occured during breadthFirstsearch\n");
	for (int i = 0; i < n; i++)
		free(tree[i]);
	free(tree);
	free(visited);
}