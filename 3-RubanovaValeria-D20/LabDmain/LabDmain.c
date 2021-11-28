#include "LabD.h"

int main()
{
	char* input = "input.txt";
	char* output = "output.txt";
	int* A = NULL;
	int B = -1;
	int N = -1;
	int subset_size = -1;
	A = readFile(input, &B, &N);
	qsort(A, N, sizeof(int), (int(*) (const void *, const void *))compare);
	int* solution = subsetSum(A, B, N, &subset_size);
	writeFile(output, solution, subset_size);
	free(solution);
	return 0;
}