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
char* searchForComma(char* line, int* k, const int i, file_line* head) {
	int j = 0;
	while (line[(*k)] != ',' && line[(*k)] != '\n' && line[(*k)] != '\0') {
		j++;
		(*k)++;
	}
	char* word = (char*)malloc((j + 1) * sizeof(char));
	if (word == NULL) {
		printf("Memory allocation error in searchForComma\n");
		return NULL;
	}
	strncpy(word, line + i, abs((*k) - i));
	word[(*k) - i] = '\0';
	return word;
}


file_line stringToStructure(char* line, file_line* head) {
	file_line element = { 0 };
	file_line err = { 0 };

	// read date (first 10 symbols are always date)
	element.date = (char*)malloc(11 * sizeof(char));
	if (element.date == NULL) {
		printf("Memory allocation error in stringToStructure\n");
		return element; // element == {0}
	}
	strncpy(element.date, line, 10);
	element.date[10] = '\0';

	//read surname
	int i = 11;
	int k = i;
	element.surname = searchForComma(line, &k, i, head);
	if (element.surname == NULL) {
		free(element.date);
		return err;
	}
	k++;
	i = k;
	element.name = searchForComma(line, &k, i, head);
	if (element.name == NULL) {
		free(element.date);
		free(element.surname);
		return err;
	}
	k++;
	i = k;

	//read hours as array of chars
	char* buf = searchForComma(line, &k, i, head);
	if (buf == NULL) {
		free(element.date);
		free(element.surname);
		free(element.name);
		return err;
	}
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
		return; // this func is void type so return is used for exiting the func
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
		return NULL;
	}
	while ((fgets(line, sizeof(line), txt))) {
		file_line* element = malloc(sizeof(file_line));
		if (element == NULL) {
			printf("Memory allocation error in readFile\n");
			destroyList(head);
			return NULL;
		}
		*element = stringToStructure(line, head);
		if (element->hours == 0 && element->date == NULL && element->name == NULL && element->surname == NULL && element->next == NULL) {
			printf("Memory allocation error in readFile\n");
			destroyList(head);
			return NULL;
		}
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


int isUnique(full_name **pArr, file_line person, file_line* head, int j) {
	full_name* arr = *pArr;
	for (int i = 0; i < j; i++) {
		if (!(strcmp(person.surname, arr[i].surname)) && !(strcmp(person.name, arr[i].name))) { // if name is already in the list, it's not unique
			return 0;
		}
	}
	// if we reached this line, the name is unique
	int i = j + 1;
	full_name* tmp = (full_name*)realloc(arr, i * sizeof(full_name));
	if (tmp == NULL) {
		printf("Memory allocation error in isUnique\n");
		return -1;
	}
	arr = tmp;
	arr[j].surname = (char*)malloc((strlen(person.surname) + 1) * sizeof(char));
	if (arr[j].surname == NULL) {
		printf("Memory allocation error in isUnique\n");
		for (int k = 0; k < j; k++) {
			free(arr[k].surname);
			free(arr[k].name);
		}
		free(arr);
		return -1;
	}
	strncpy(arr[j].surname, person.surname, strlen(person.surname));
	arr[j].surname[strlen(person.surname)] = '\0';
	arr[j].name = (char*)malloc((strlen(person.name) + 1) * sizeof(char));
	if ((arr + j)->name == NULL) {
		printf("Memory allocation error in isUnique\n");
		for (int k = 0; k < j; k++) {
			free(arr[k].surname);
			free(arr[k].name);
		}
		free(arr[j].surname);
		return -1;
	}
	strncpy((arr + i - 1)->name, person.name, strlen(person.name));
	arr[j].name[strlen(person.name)] = '\0';
	*pArr = arr;
	return 1;
}


int countSum(file_line* list, char* surname, char* name) {
	int sum_hours = 0;
	while (list != NULL) {
		if (!(strcmp(list->surname, surname)) && !(strcmp(list->name, name))) { // if we found the same person in the list
			sum_hours += list->hours;
		}
		list = list->next;
	}
	return sum_hours;
}


char* printInfo(file_line* list, int n) {
	file_line* head = list;
	file_line* list_copy = list;
	full_name* arr = (full_name*)malloc(sizeof(full_name));
	int j = 0;
	char* string = (char*)malloc(sizeof(char));
	int k = 0;
	while (list_copy != NULL) {
		if (isUnique(&arr, (*list_copy), head, j) == 1) {
			j++;
			int sum_hours = countSum(list, list_copy->surname, list_copy->name);
			if (sum_hours > n) {
				char* tmp = (char*)realloc(string, (k + strlen(list_copy->surname) + strlen(list_copy->name) + 2) * sizeof(char));
				if (tmp == NULL) {
					printf("Memory allocation error in isUnique\n");
					free(string);
					return NULL;
				}
				string = tmp;
				strncpy(string + k, list_copy->surname, strlen(list_copy->surname));
				k += strlen(list_copy->surname);
				string[k] = ' ';
				k++;
				strncpy(string + k, list_copy->name, strlen(list_copy->name));
				k += strlen(list_copy->name);
				string[k] = '\n';
				k++;
			}
		}
		else if (isUnique(&arr, (*list_copy), head, j) == -1) {
			return NULL;
		}
		list_copy = list_copy->next;
	}
	char* tmp = (char*)realloc(string, (k + 1) * sizeof(char));
	if (tmp == NULL) {
		printf("Memory allocation error in isUnique\n");
		free(string);
		return NULL;
	}
	string = tmp;
	string[k] = '\0';
	for (int t = 0; t < j; t++) {
		free(arr[t].surname);
		free(arr[t].name);
	}
	free(arr);
	return string;
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