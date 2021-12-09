#include "LabD.h"

int main()
{
	char* input = "input.txt";
	char* output = "output.txt";
	if (labSolution(input, output) == -1) {
		printf("Oops it seems that error ocurred. Sorry!\n");
	}
	else {
		printf("Check file, solution is there already!\n");
	}
	return 0;
}