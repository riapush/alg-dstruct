#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "laba_header.h"

#pragma warning(disable : 4996)


int charToInt(char a) {
	if (a >= '0' && a <= '9') {
		return a - '0';
	}
	else {
		return -1; // not a number
	}
}


// transform char hours to int 
void makeNumOutOfDigits(file_line* element, char* buf) {
	int len = strlen(buf);
	int pwr = len - 1;
	element->hours = 0;

	for (int n = 0; n < len; n++) {
		int digit = charToInt(buf[n]);
		element->hours += digit * (int)pow(10, pwr);
		pwr--;
	}
}

file_line stringToStructure(char* line, file_line* head) {
	int i = 0;
	int j = 0;
	file_line element = { 0 };

	// read date
	element.date = (char*)malloc(11 * sizeof(char));
	if (element.date == NULL) {
		printf("Memory allocation error in stringToStructure");
		destroyList(head);
		return;
	}
	while (line[i] != ',') {
		j++;
		i++;
		element.date[j - 1] = line[i - 1];
	}
	j++;
	element.date[j - 1] = '\0';
	i++; //skip ','

	//read surname
	j = 0;
	int k = i;
	while (line[k] != ',') {
		j++;
		k++;
	}
	element.surname = (char*)malloc((j+1) * sizeof(char));
	j = 0;
	while (line[i] != ',') {
		i++;
		j++;
		element.surname[j - 1] = line[i - 1];
	}
	j++;
	element.surname[j - 1] = '\0';
	i++; //skip ','

	//read name
	j = 0;
	k = i;
	while (line[k] != ',') {
		j++;
		k++;
	}
	element.name = (char*)malloc((j + 1) * sizeof(char));
	if (element.name == NULL) {
		printf("Memory allocation error in stringToStructure");
		destroyList(head);
		return;
	}
	j = 0;
	while (line[i] != ',') {
		i++;
		j++;
		element.name[j - 1] = line[i - 1];
	}
	j++;
	element.name[j - 1] = '\0';
	i++; //skip ','

	j = 0;
	k = i;
	//read hours as array of chars
	char* buf = NULL;
	while (line[k] != '\n' && line[k] != '\0') {
		k++;
		j++;
	}
	buf = (char*)malloc((j+1) * sizeof(char));
	if (buf == NULL) {
		printf("Memory allocation error in stringToStructure");
		destroyList(head);
		return;
	}
	j = 0;
	while (i <= k && line[i]!='\n' && line[i]!='\0') {
		i++;
		j++;
		buf[j - 1] = line[i - 1];
	}
	j++;
	buf[j - 1] = '\0';
	makeNumOutOfDigits(&element, buf);
	free(buf);
	return element; // "next" field is gonna be defined in FindElementPosition function
}


//file_line stringToStructure(char* line, file_line* head) {
//	int len = strlen(line);
//	int i = 0;
//	int j = 0;
//	file_line element = { 0 };
//
//	// read date
//	while (line[i] != ',') {
//		j++;
//		i++;
//		element.date = (char*)realloc(element.date, j * sizeof(char));
//		if (element.date == NULL) {
//			printf("Memory allocation error in stringToStructure");
//			destroyList(head);
//			return;
//		}
//		element.date[j - 1] = line[i - 1];
//	}
//	j++;
//	element.date = (char*)realloc(element.date, j * sizeof(char));
//	if (element.date == NULL) {
//		printf("Memory allocation error in stringToStructure");
//		destroyList(head);
//		return;
//	}
//	element.date[j - 1] = '\0';
//	i++; //skip ','
//	j = 0;
//	//read surname
//	while (line[i] != ',') {
//		i++;
//		j++;
//		element.surname = (char*)realloc(element.surname, j * sizeof(char));
//		if (element.surname == NULL) {
//			printf("Memory allocation error in stringToStructure");
//			destroyList(head);
//			return;
//		}
//		element.surname[j - 1] = line[i - 1];
//	}
//	j++;
//	element.surname = (char*)realloc(element.surname, j * sizeof(char));
//	if (element.surname == NULL) {
//		printf("Memory allocation error in stringToStructure");
//		destroyList(head);
//		return;
//	}
//	element.surname[j - 1] = '\0';
//	i++; //skip ','
//	j = 0;
//	//read name
//	while (line[i] != ',') {
//		i++;
//		j++;
//		element.name = (char*)realloc(element.name, j * sizeof(char));
//		if (element.name == NULL) {
//			printf("Memory allocation error in stringToStructure");
//			destroyList(head);
//			return;
//		}
//		element.name[j - 1] = line[i - 1];
//	}
//	j++;
//	element.name = (char*)realloc(element.name, j * sizeof(char));
//	if (element.name == NULL) {
//		printf("Memory allocation error in stringToStructure");
//		destroyList(head);
//		return;
//	}
//	element.name[j - 1] = '\0';
//	i++; //skip ','
//	j = 0;
//	//read hours as array of chars
//	char* buf = NULL;
//	while (line[i] != '\n' && line[i] != '\0') {
//		i++;
//		j++;
//		buf = (char*)realloc(buf, j * sizeof(char));
//		if (buf == NULL) {
//			printf("Memory allocation error in stringToStructure");
//			destroyList(head);
//			return;
//		}
//		buf[j - 1] = line[i - 1];
//	}
//	j++;
//	buf = (char*)realloc(buf, j * sizeof(char));
//	if (buf == NULL) {
//		printf("Memory allocation error in stringToStructure");
//		destroyList(head);
//		free(buf);
//		return;
//	}
//	buf[j - 1] = '\0';
//	makeNumOutOfDigits(&element, buf);
//	free(buf);
//	return element; // "next" field is gonna be defined in FindElementPosition function
//}



//return: 1 if strings are equal, 2 if the first string should be put before second when sort in alphabet order, else 0
int compareStrings(char* A, char* B) {
	int a_len = strlen(A);
	int b_len = strlen(B);
	int len = (a_len <= b_len) ? a_len : b_len;
	for (int i = 0; i < len; i++) {
		if ((int)A[i] < (int)B[i]) {
			return 2;
		}
		if ((int)A[i] > (int)B[i]) {
			return 0;
		}
	}
	// if we reach this line, we have either equal strings or one string is shorter and the other includes it
	if (a_len < b_len) {
		return 2;
	}
	if (a_len == b_len) {
		return 1;
	}
	return 0;
}


int headSmaller(file_line* head, file_line* element) {
	/*if element needs to be put first, we need to change pointer to the head of a list
	return: 1 if head smaller, 0 else
	*/
	if (element->hours > head->hours) {
		return 1;
	}
	else if (element->hours == head->hours) {
		if (compareStrings(element->surname, head->surname) == 1) {
			if (compareStrings(element->name, head->name) == 1 || compareStrings(element->name, head->name) == 2) {
				return 1;
			}
		}
		else if (compareStrings(element->surname, head->surname) == 2) {
			return 1;
		}
	}
	return 0;
}

void putBefore(file_line** prev, file_line** curr, file_line** next) {
	(*prev)->next = (*curr);
	(*curr)->next = (*next);
}

void putTheVeryLast(file_line** element, file_line** other_element) {
	(*other_element)->next = (*element);
	(*element)->next = NULL;
}

void goOn(file_line** previous, file_line** other_element) {
	(*previous) = (*other_element);
	(*other_element) = (*other_element)->next;
}

//function sorts elements from max working time to min working time 
void findElementPosition(int element_counter, file_line* element, file_line* other_element) {
	file_line* previous = NULL;
	int i = 1; // counter of on what element we are now
	if (element_counter == 0) {
		element->next = NULL;
		return;
	}
	else {
		do {
			if (element->hours > other_element->hours) {
				putBefore(&previous, &element, &other_element);
				return;
			}
			else if (element->hours == other_element->hours) {
				// if hours are equal, we need to compare surnames and (sometimes) names)
				if (compareStrings(element->surname, other_element->surname) == 1) {
					// if surnames are the same, we need to compare names
					if (compareStrings(element->name, other_element->name) == 1) {
						// if names are equal, we do not care where to put
						putBefore(&previous, &element, &other_element);
						return;
					}
					else if (compareStrings(element->name, other_element->name) == 2) {
						putBefore(&previous, &element, &other_element);
						return;
					}
					else {
						// we need to go as long as we can so we put element only before, not after
						if (i < element_counter) {
							goOn(&previous, &other_element);
							i++;
						}
						else {
							putTheVeryLast(&element, &other_element);
							return;
						}
					}
				}
				else if (compareStrings(element->surname, other_element->surname) == 2) {
					putBefore(&previous, &element, &other_element);
					return;
				}
				else {
					if (i < element_counter) {
						goOn(&previous, &other_element);
						i++;
					}
					else {
						putTheVeryLast(&element, &other_element);
						return;
					}
				}
			}
			else {
				if (i < element_counter) {
					goOn(&previous, &other_element);
					i++;
				}
				else {
					putTheVeryLast(&element, &other_element);
					return;
				}
			}
		} while (1);

	}
}

file_line* readFile(const char* filename) {
	FILE* txt = fopen(filename, "r");
	if (!txt) {
		printf("Unable to open file in readFile");
		return NULL;
	}
	char line[200];
	int elements_counter = 0;
	file_line* head = malloc(sizeof(file_line));
	if (head == NULL) {
		printf("Memory allocation error in readFile");
		return;
	}
	// [^\n] reads until newline
	// 199 is character limit
	while ((fgets(line, sizeof(line), txt))) {
		file_line* element = malloc(sizeof(file_line));
		if (element == NULL) {
			printf("Memory allocation error in stringToStructure");
			free(element);
			destroyList(head);
			return;
		}
		*element = stringToStructure(line, head);
		if (elements_counter == 0) {
			head = element;
			elements_counter++;
			continue;
		}

		if (headSmaller(head, element)) {
			element->next = head;
			head = element;
		}
		else {
			findElementPosition(elements_counter, element, head);
		}
		elements_counter++;
	}
	return head;
}


void printInfo(file_line* list, int n) {
	printf("Worked more than %i hours:\n", n);
	while (list->next != NULL) {
		if (list->hours > n) {
			for (int i = 0; i < (int)strlen(list->date); i++) {
				printf("%c", list->date[i]);
			}
			printf(": ");
			for (int i = 0; i < (int)strlen(list->surname); i++) {
				printf("%c", list->surname[i]);
			}
			printf("\n");
		}
	}
}

void destroyList(file_line* head) {
	file_line* A = head->next;
	free(head);
	while (A != NULL) {
		file_line B = *A;
		free(A);
		A = B.next;
	}
}