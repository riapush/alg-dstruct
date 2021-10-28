#pragma once

int* sizeCopy(int* element);


int** nextOf(int* element);

int** prevOf(int* element);

int* isFree(int* element);

typedef struct init {
	int* ptr;
	int size;

}init;


extern init list;
