#include "pch.h"
#include "LabC.h"
#include <time.h>

TEST(bfsStressTest, stressTest) {
	unsigned long start = clock();
	int num_v = 15000;
	if (tree != NULL) {
		free(tree);
	}
	tree = (int**)malloc(sizeof(int*) * num_v);
	ASSERT_TRUE(tree != NULL);
	for (int i = 0; i < num_v; i++) {
		tree[i] = (int*)malloc(sizeof(int) * num_v);
		if (tree[i] == NULL) {
			for (int j = i - 1; j >= 0; j--)
				free(tree[j]);
			free(tree);
		}
		ASSERT_TRUE(tree != NULL);
	}
	for (int i = 0; i < num_v; i++) {
		for (int j = 0; j < num_v; j++) {
			tree[i][j] = rand() % 2;
			tree[j][i] = tree[i][j];
		}
	}

	visited = (int*)malloc(sizeof(int) * num_v);
	if (visited == NULL) {
		for (int i = 0; i < n; i++)
			free(tree[i]);
		free(tree);
		ASSERT_TRUE(visited != NULL);
	}
	for (int i = 0; i < num_v; i++)
		visited[i] = 0;

	EXPECT_TRUE(breadthFirstSearch(0) == 1);
	double time = (double)(clock() - start) / CLOCKS_PER_SEC;
	printf("%f\n", time);

	for (int i = 0; i < n; i++)
		free(tree[i]);
	free(tree);
	free(visited);
}