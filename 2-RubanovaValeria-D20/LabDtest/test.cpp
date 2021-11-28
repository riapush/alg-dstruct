#include "pch.h"
#include "LabD.h"
#include <time.h>
#include <math.h>

TEST(readFile, expectedValidValue) {
	char* input = "reading_test.txt";
	int* A = NULL;
	int B = -1;
	int N = -1;
	A = readFile(input, &B, &N);
	EXPECT_EQ(B, 9);
	EXPECT_EQ(N, 6);

	EXPECT_EQ(A[0], 2);
	EXPECT_EQ(A[1], 3);
	EXPECT_EQ(A[2], 4);
	EXPECT_EQ(A[3], 5);
	EXPECT_EQ(A[4], 12);
	EXPECT_EQ(A[5], 34);

	free(A);
}

TEST(fillMatrix, sum4_set123_expectedValidValue) {
	int* set = (int*)malloc(3 * sizeof(int));
	ASSERT_TRUE(set != NULL);
	set[0] = 1;
	set[1] = 2;
	set[2] = 3;
	bool** A = createMatrix(3,5);
	ASSERT_TRUE(A != NULL);
	fillMatrix(A, set, 3, 5);

	EXPECT_EQ(A[0][0], 1);
	EXPECT_EQ(A[1][0], 1);
	EXPECT_EQ(A[2][0], 1);

	EXPECT_EQ(A[0][1], 1);
	EXPECT_EQ(A[1][1], 1);
	EXPECT_EQ(A[2][1], 1);

	EXPECT_EQ(A[0][2], 0);
	EXPECT_EQ(A[1][2], 1);
	EXPECT_EQ(A[2][2], 1);

	EXPECT_EQ(A[0][3], 0);
	EXPECT_EQ(A[1][3], 1);
	EXPECT_EQ(A[2][3], 1);

	EXPECT_EQ(A[0][4], 0);
	EXPECT_EQ(A[1][4], 0);
	EXPECT_EQ(A[2][4], 1);

	freeMatrix(A, 3);
}

TEST(traceSubset, sum4_set123_expected31) {
	int* set = (int*)malloc(3 * sizeof(int));
	ASSERT_TRUE(set != NULL);
	set[0] = 1;
	set[1] = 2;
	set[2] = 3;
	bool** A = createMatrix(3, 5);
	A[0][0] = 1;
	A[1][0] = 1;
	A[2][0] = 1;

	A[0][1] = 1;
	A[1][1] = 1;
	A[2][1] = 1;

	A[0][2] = 0;
	A[1][2] = 1;
	A[2][2] = 1;

	A[0][3] = 0;
	A[1][3] = 1;
	A[2][3] = 1;

	A[0][4] = 0;
	A[1][4] = 0;
	A[2][4] = 1;

	int subset_size = -1;
	int* solution = traceSubset(A, set, 3, 5, &subset_size);
	EXPECT_EQ(subset_size, 2);
	EXPECT_EQ(solution[0], 3);
	EXPECT_EQ(solution[1], 1);

	free(solution);
	freeMatrix(A, 3);
}

TEST(subsetSum, sum4_set123_expected31) {
	int* set = (int*)malloc(3 * sizeof(int));
	ASSERT_TRUE(set != NULL);
	set[0] = 1;
	set[1] = 2;
	set[2] = 3;
	int subset_size = -1;
	int* solution = subsetSum(set, 4, 3, &subset_size);

	EXPECT_EQ(subset_size, 2);
	EXPECT_EQ(solution[0], 3);
	EXPECT_EQ(solution[1], 1);

	free(solution);
}

TEST(subsetSum, sum7_set123_expected0) {
	int* set = (int*)malloc(3 * sizeof(int));
	ASSERT_TRUE(set != NULL);
	set[0] = 1;
	set[1] = 2;
	set[2] = 3;
	int subset_size = -1;
	int* solution = subsetSum(set, 7, 3, &subset_size);

	EXPECT_EQ(subset_size, 1);
	EXPECT_EQ(solution[0], 0);

	free(solution);
}

TEST(subsetSum, stressTest) {
	unsigned long start = clock();
	int n = 999;
	int* set = (int*)malloc(n * sizeof(int));
	ASSERT_TRUE(set != NULL);
	for (int i = 0; i < n-1; i++) {
		set[i] = rand() % (int)pow(2,16);
	}
	set[n - 1] = 1;
	int subset_size = -1;
	int* solution = subsetSum(set, 1, n, &subset_size);
	EXPECT_EQ(subset_size, 1);
	EXPECT_EQ(solution[0], 1);

	double time = (double)(clock() - start) / CLOCKS_PER_SEC;
	printf("%f\n", time);
}