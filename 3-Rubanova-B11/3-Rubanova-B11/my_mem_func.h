#pragma once

int** prevOf(int* element);

int** nextOf(int* element);

int* flagOf(int* element);

int* sizeCopy(int* element);

typedef struct init {
	int* ptr;
	int size;

}init;


extern init list;
