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

int CharToInt(char a);

void MakeNumOutOfDigits(file_line* element, char* buf);

file_line StringToStructure(char* line);

int CompareStrings(char* A, char* B);

int HeadSmaller(file_line* head, file_line* element);

void PutBefore(file_line** prev, file_line** curr, file_line** next);

void PutTheVeryLast(file_line** element, file_line** other_element);

void GoOn(file_line** previous, file_line** other_element);

void FindElementPosition(int element_counter, file_line* element, file_line* other_element);

file_line* ReadFile(char* filename);

void PrintInfo(file_line* list, int n);

#ifdef __cplusplus
}
#endif

#endif