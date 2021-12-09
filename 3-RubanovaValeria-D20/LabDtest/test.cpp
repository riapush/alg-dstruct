#include "pch.h"
#include "LabD.h"
#include <time.h>
#include <math.h>
#define MAX_LENGTH 999

TEST(readFile, expectedValidValue) {
	char* input = "reading_test.txt";
	int* A = NULL;
	long int B = -1;
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

const int count = 100;
/*
Device name	LAPTOP-F07HB6MN
CPU	AMD Ryzen 5 3500U with Radeon Vega Mobile Gfx     2.10 GHz
RAM	8,00 GB (free: 6,94 GB)

time: 10.688000 sec
memory: about 848 mb
*/
TEST(stressTest, labSolution) {
	char* input = "stress_test.txt";
	char* output = "stress_test_output.txt";
	FILE* fstdin = fopen(input, "w");
	srand(time(NULL));
	if (fstdin == NULL) {
		printf("Error in file");
		ASSERT_TRUE(fstdin);
	}
	int sum = rand() % MAX_LENGTH;
	// fill file 
	fprintf(fstdin, "%d\n", sum);
	fprintf(fstdin, "%d\n", count);
	for (int i = 0; i < count; i++) {
		fprintf(fstdin, "%d ", rand() % MAX_LENGTH);
	}
	fclose(fstdin);
	unsigned long start = clock();
	int k = labSolution("stress_test.txt", "stress_test_output.txt");
	double time = (double)(clock() - start) / CLOCKS_PER_SEC;
	EXPECT_EQ(k, 0);
	printf("%lf\n", time);
}

TEST(functionalTest, sum7_set123_expected0) {
	char* filename = "test1_input.txt";
	char* output = "test1_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "0 ");
	fclose(fp);
}

TEST(functionalTest, sum4_set123_expected13) {
	char* filename = "test2_input.txt";
	char* output = "test2_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "1 3 ");
	fclose(fp);
}

TEST(functionalTest, sum9_expected234) {
	char* filename = "test3_input.txt";
	char* output = "test3_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "2 3 4 ");
	fclose(fp);
}

TEST(functionalTest, sum10_expected235) {
	char* filename = "test4_input.txt";
	char* output = "test4_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "2 3 5 ");
	fclose(fp);
}

TEST(functionalTest, sum20_expected12458) {
	char* filename = "test5_input.txt";
	char* output = "test5_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "0 1 2 4 5 8 ");
	fclose(fp);
}

TEST(functionalTest, sum7_expected133) {
	char* filename = "test6_input.txt";
	char* output = "test6_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "1 3 3 ");
	fclose(fp);
}

TEST(functionalTest, sum20_expected223445) {
	char* filename = "test7_input.txt";
	char* output = "test7_output.txt";
	int to_check = labSolution(filename, output);
	FILE* fp = fopen(output, "r");
	if (fp == NULL) {
		printf("error in file");
		ASSERT_TRUE(fp);
	}
	char str[100];
	fgets(str, 100, fp);
	EXPECT_STREQ(str, "0 2 2 3 4 4 5 ");
	fclose(fp);
}