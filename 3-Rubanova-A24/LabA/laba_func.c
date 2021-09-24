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


// func search for comma and allocate memory for found word
char* searchForComma(char* line, int* j, int* k) {
	while (line[(*k)] != ',' && line[(*k)] != '\n' && line[(*k)] != '\0') {
		(*j)++;
		(*k)++;
	}
	return (char*)malloc(((*j) + 1) * sizeof(char));
}

file_line stringToStructure(char* line, file_line* head) {
	int i = 0;
	int j = 0;
	file_line element = { 0 };

	// read date (first 10 symbols are always date)
	element.date = (char*)malloc(11 * sizeof(char));
	if (element.date == NULL) {
		printf("Memory allocation error in stringToStructure\n");
		destroyList(head);
		return;
	}
	while (i < 10) {
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
	element.surname = searchForComma(line, &j, &k);
	if (element.surname == NULL) {
		printf("Memory allocation error in stringToStructure\n");
		destroyList(head);
		return;
	}
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
	element.name = searchForComma(line, &j, &k);
	if (element.name == NULL) {
		printf("Memory allocation error in stringToStructure\n");
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
	char* buf = searchForComma(line, &j, &k);
	if (buf == NULL) {
		printf("Memory allocation error in stringToStructure\n");
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


int oneBeforeOther(file_line* head, file_line* element) {
	// return: 1 if we need to put our element before other, 0 else
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
			if (oneBeforeOther(other_element, element)) {
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
		} while (1);

	}
}


file_line* readFile(const char* filename) {
	FILE* txt = fopen(filename, "r");
	if (!txt) {
		printf("Unable to open file in readFile\n");
		return NULL;
	}
	fseek(txt, 0, SEEK_END);
	long pos = ftell(txt);
	if (pos == 0) { // file is empty
		printf("File is empty\n");
		return NULL;
	}
	else {
		rewind(txt);
	}
	char line[200];
	int elements_counter = 0;
	file_line* head = malloc(sizeof(file_line));
	if (head == NULL) {
		printf("Memory allocation error in readFile\n");
		return;
	}
	// [^\n] reads until newline
	// 199 is character limit
	while ((fgets(line, sizeof(line), txt))) {
		file_line* element = malloc(sizeof(file_line));
		if (element == NULL) {
			printf("Memory allocation error in stringToStructure\n");
			free(element);
			destroyList(head);
			return;
		}
		*element = stringToStructure(line, head);
		if (elements_counter == 0) {
			free(head);
			head = element;
			elements_counter++;
			continue;
		}

		if (oneBeforeOther(head, element)) {
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

int findPerson(file_line* list, char* surname, char* name) {
	int sum_hours = 0;
	while (list != NULL) {
		if (!(strcmp(list->surname, surname) && strcmp(list->name, name))) { // if we found the same person in the list
			sum_hours += list->hours;
		}
		list = list->next;
	}
	return sum_hours;
}


void printInfo(file_line* list, int n) {
	file_line* head = list;
	while (head != NULL) {
		char* surname = head->surname;
		char* name = head->name;
		int sum_hours = findPerson(list, surname, name);
		if (sum_hours > n) {
			for (int i = 0; i < (int)strlen(head->surname); i++) {
				printf("%c", head->surname[i]);
			}
			printf("\n");
		}
		head = head->next;
	}
}


void destroyElement(file_line* A) {
	free(A->date);
	free(A->surname);
	free(A->name);
	free(A);
}


void destroyList(file_line* head) {
	file_line* A = head->next;
	while (A != NULL) {
		destroyElement(head);
		head = A;
		A = head->next;
	}
	//when A == NULL, we have 1 element left.
	destroyElement(head);
}