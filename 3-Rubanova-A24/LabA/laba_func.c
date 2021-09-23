#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "laba_header.h"


int CharToInt(char a) {
	if (a >= '0' && a <= '9') {
		return a - '0';
	}
	else {
		return -1; // not a number
	}
}


// transform char hours to int 
void MakeNumOutOfDigits(file_line* element, char* buf) {
	int len = strlen(buf);
	int pwr = len - 1;
	element->hours = 0;

	for (int n = 0; n < len; n++) {
		int digit = CharToInt(buf[n]);
		element->hours += digit * (int)pow(10, pwr);
		pwr--;
	}
}


file_line StringToStructure(char* line) {
	int len = strlen(line);
	int i = 0;
	int j = 0;
	file_line element = { 0 };

	// string.h strtok can separate string into multiple based on character delimiters
	// in this case delimiter is ','
	// WARNING damages initial string
	// read date
	while (line[i] != ',') {
		j++;
		i++;
		element.date = (char*)realloc(element.date, j * sizeof(char));
		element.date[j - 1] = line[i - 1];
	}
	j++;
	element.date = (char*)realloc(element.date, j * sizeof(char));
	element.date[j - 1] = '\0';
	i++; //skip ','
	j = 0;
	//read surname
	while (line[i] != ',') {
		i++;
		j++;
		element.surname = (char*)realloc(element.surname, j * sizeof(char));
		element.surname[j - 1] = line[i - 1];
	}
	j++;
	element.surname = (char*)realloc(element.surname, j * sizeof(char));
	element.surname[j - 1] = '\0';
	i++; //skip ','
	j = 0;
	//read name
	while (line[i] != ',') {
		i++;
		j++;
		element.name = (char*)realloc(element.name, j * sizeof(char));
		element.name[j - 1] = line[i - 1];
	}
	j++;
	element.name = (char*)realloc(element.name, j * sizeof(char));
	element.name[j - 1] = '\0';
	i++; //skip ','
	j = 0;
	//read hours as array of chars
	char* buf = NULL;
	while (line[i] != '\n' && line[i] != '\0') {
		i++;
		j++;
		buf = (char*)realloc(buf, j * sizeof(char));
		buf[j - 1] = line[i - 1];
	}
	j++;
	buf = (char*)realloc(buf, j * sizeof(char));
	buf[j - 1] = '\0';
	MakeNumOutOfDigits(&element, buf);
	return element; // "next" field is gonna be defined in FindElementPosition function
}

// string.h strcmp does string comparison, result is <0 if first string is "less", 0 if strings are equal and >0 if first string is "greater"
//return: 1 if strings are equal, 2 if the first string should be put before second when sort in alphabet order, else 0
int CompareStrings(char* A, char* B) {
	int A_len = strlen(A);
	int B_len = strlen(B);
	int len = (A_len <= B_len) ? A_len : B_len;
	for (int i = 0; i < len; i++) {
		if ((int)A[i] < (int)B[i]) {
			return 2;
		}
		if ((int)A[i] > (int)B[i]) {
			return 0;
		}
	}
	// if we reach this line, we have either equal strings or one string is shorter and the other includes it
	if (A_len < B_len) {
		return 2;
	}
	if (A_len == B_len) {
		return 1;
	}
	return 0;
}


/*if element needs to be put first, we need to change pointer to the head of a list
return: 1 if head smaller, 0 else
*/
int HeadSmaller(file_line* head, file_line* element) {
	if (element->hours > head->hours) {
		return 1;
	}
	else if (element->hours == head->hours) {
		if (CompareStrings(element->surname, head->surname) == 1) {
			if (CompareStrings(element->name, head->name) == 1 || CompareStrings(element->name, head->name) == 2) {
				return 1;
			}
		}
		else if (CompareStrings(element->surname, head->surname) == 2) {
			return 1;
		}
	}
	return 0;
}

// PutBefore and PutTheVeryLast can be generalized by using
// curr->next = prev->next;
// prev->next = curr;
// no need for double pointers
void PutBefore(file_line** prev, file_line** curr, file_line** next) {
	(*prev)->next = (*curr);
	(*curr)->next = (*next);
}

void PutTheVeryLast(file_line** element, file_line** other_element) {
	(*element)->next = NULL;
	(*other_element)->next = (*element);
}

void GoOn(file_line** previous, file_line** other_element) {
	(*previous) = (*other_element);
	(*other_element) = (*other_element)->next;
}

//function sorts elements from max working time to min working time 
void FindElementPosition(int element_counter, file_line* element, file_line* other_element) {
	file_line* previous = NULL;
	int i = 1; // counter of on what element we are now
	if (element_counter == 0) {
		element->next = NULL;
		return;
	}
	else {
		// we can not to compare to the head bc we are sure element needs to be put after so we skip head
		/*previous = other_element;
		other_element = other_element->next;
		i++;*/

		do {
			if (element->hours > other_element->hours) {
				PutBefore(&previous, &element, &other_element);
				return;
			}
			else if (element->hours == other_element->hours) {
				// if hours are equal, we need to compare surnames and (sometimes) names)
				if (CompareStrings(element->surname, other_element->surname) == 1) {
					// if surnames are the same, we need to compare names
					if (CompareStrings(element->name, other_element->name) == 1) {
						// if names are equal, we do not care where to put
						PutBefore(&previous, &element, &other_element);
						return;
					}
					else if (CompareStrings(element->name, other_element->name) == 2) {
						PutBefore(&previous, &element, &other_element);
						return;
					}
					else {
						// we need to go as long as we can so we put element only before, not after
						if (i < element_counter) {
							GoOn(&previous, &other_element);
							i++;
						}
						else {
							PutTheVeryLast(&element, &other_element);
							return;
						}
					}
				}
				else if (CompareStrings(element->surname, other_element->surname) == 2) {
					PutBefore(&previous, &element, &other_element);
					return;
				}
				else {
					if (i < element_counter) {
						GoOn(&previous, &other_element);
						i++;
					}
					else {
						PutTheVeryLast(&element, &other_element);
						return;
					}
				}
			}
			else {
				if (i < element_counter) {
					GoOn(&previous, &other_element);
					i++;
				}
				else {
					PutTheVeryLast(&element, &other_element);
					return;
				}
			}
		} while (other_element->next != NULL);

	}
}


// changed filename to be const so there's no C++ warning
// also its generally useful to use const if there's no plan to change something to avoid accidental changes
file_line* ReadFile(const char* filename) {
	FILE* txt = fopen(filename, "r");
	if (!txt) {
		perror("Unable to open file in ReadFile");
		return NULL;
	}
	char line[200];
	int elements_counter = 0;
	file_line* head = malloc(sizeof(file_line));

	// [^\n] reads until newline
	// 199 is character limit
	// \n after string is needed to read more than one line (consumes leftover newline character)
	while (fscanf(txt, "%199[^\n]\n", line)>0) {
		file_line* element = malloc(sizeof(file_line*));
		*element = StringToStructure(line);
		if (elements_counter == 0) {
			head = element;
		}

		if (HeadSmaller(head, element)) {
			element->next = head;
			head = element;
		}
		else {
			FindElementPosition(elements_counter, element, head);
		}
		elements_counter++;
	}
	return head;
}


void PrintInfo(file_line* list, int n) {
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
