#pragma once
#ifndef _MY_H 
#define _MY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct file_line {
	char* date;
	char* surname;
	char* name;
	int hours;
	struct file_line* next;
} file_line;

typedef struct full_name {
	char* surname;
	char* name;
}full_name;

int charToInt(char a);

void makeNumOutOfDigits(file_line* element, char* buf);

void destroyList(file_line* head);

file_line stringToStructure(char* line, file_line* head);

int compareStrings(char* A, char* B);

int countSum(file_line* list, char* surname, char* name);

int oneBeforeOther(file_line* head, file_line* element);

void putBefore(file_line** prev, file_line** curr, file_line** next);

void putTheVeryLast(file_line** element, file_line** other_element);

void goOn(file_line** previous, file_line** other_element);

void findElementPosition(int element_counter, file_line* element, file_line* other_element);

file_line* readFile(char* filename);

int isUnique(full_name* arr, file_line person, file_line* head, int j);

int countSum(file_line* list, char* surname, char* name);

void printInfo(file_line* list, file_line* head, int n);

#ifdef __cplusplus
}
#endif

#endif