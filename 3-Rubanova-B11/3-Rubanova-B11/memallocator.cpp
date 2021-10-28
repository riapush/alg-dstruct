#include "memallocator.h"
#include <stdlib.h>
#include <stdio.h>
#define OK 1
#define ERROR -1
#define FALSE (int)0
#define TRUE (int)1
#define DESC_SIZE (3 * sizeof(int) + 2 * sizeof(int*)) // descriptor size


typedef struct init {
	int* ptr;
	int size;

}init;


init list;
void *pMemory1;


int** prevOf(int* element) {
	return (int**)((char*)element + sizeof(int) + sizeof(int*));
}


int** nextOf(int* element) {
	return (int**)((char*)element + sizeof(int));
}


int* isFree(int* element) {
	return (int*)((char*)element + sizeof(int) + 2 * sizeof(int*));
}


int* sizeCopy(int* element) {
	return (int*)((char*)element + *element - sizeof(int));
}


int meminit(void *pMemory, int size) {
	// descriptor consist of size = size, next = NULL, prev = NULL, is used = FALSE, sizecopy in the end
	if (size <= memgetminimumsize()) // 0-size block musn't be allocated
		return 0;
	pMemory1 = pMemory;
	list.size = size;
	list.ptr = (int*)pMemory;
	*(list.ptr) = size;
	*nextOf(list.ptr) = (int*)NULL; // next 
	*prevOf(list.ptr) = (int*)NULL; // prev
	*isFree(list.ptr) = TRUE; // is used
	*sizeCopy(list.ptr) = size; // copy size in the end of block
	return OK;
}


void* memalloc(int size) {
	if ((int)size < 1 || (int)size > list.size - memgetblocksize())
		return NULL;
	int* head = list.ptr;
	if (head == (int*)NULL) {
		return NULL;
	}
	while (*head < (int)(size + DESC_SIZE)) { // while suitable block not found
		if (*nextOf(head) == (int*)NULL) { // if next == NULL, there's no free blocks left
			return NULL;
		}
		head = *nextOf(head); // move to next block
	}
	// if we reached this line, we found suitable block
	// check for fragmentation
	if (*head - (size + DESC_SIZE) > DESC_SIZE) {
		// we can have 2 blocks
		int* next = (int*)((char*)head + DESC_SIZE + size);
		*next = *head - (size + DESC_SIZE); // size of next equals to size of current block without taken memory
		*sizeCopy(next) = *next; // size copy in the end
		*nextOf(next) = *nextOf(head); // next.next equals head.next
		*prevOf(next) = *prevOf(head); // next.prev equals head.prev
		*isFree(next) = TRUE; // is used
		if (*prevOf(head) != (int*)NULL) { // if head had prev element
			int* prev = *prevOf(head); //we need to change prev.next from head to next
			*nextOf(prev) = next;
		}
		else {
			list.ptr = next;
		}
		if (*nextOf(head) != (int*)NULL) {
			int* next1 = *nextOf(head); //we need to change next.next from head to prev
			*prevOf(next1) = next;
		}
		*head = size + DESC_SIZE;
		*sizeCopy(head) = *head; // size copy in the end

	}
	else { // then we have fragmentation. to escape that situation we don't divide block
		// head.size doesn't change
		// just delete head out of list
		int* next = *nextOf(head);
		int* prev = *prevOf(head);
		if (next == (int*)NULL && prev == (int*)NULL) {
			list.ptr = NULL; // list of free blocks is empty 
		}
		else if (prev == (int*)NULL) {
			*prevOf(next) = (int*)NULL; // then next1 becomes first
			list.ptr = next;
		}
		else if (next == (int*)NULL) {
			*nextOf(prev) = (int*)NULL; // if next == null, then prev becomes last
		}
		else {
			*nextOf(prev) = next;
			*prevOf(next) = prev;
		}
	}
	*isFree(head) = FALSE; // is used
	*nextOf(head) = (int*)NULL; // head is occupied so it's out of list
	*prevOf(head) = (int*)NULL;
	return (void*)((char*)head + DESC_SIZE - sizeof(int)); // first 4 cells are occupied w/ descriptor
}


void memfree(void *p) {
	if (p == NULL) {
		return;
	}
	int merged_w_left = FALSE;
	int merged_w_right = FALSE;
	int* head = (int*)((char*)p - DESC_SIZE + sizeof(int)); // go to descriptor
	*isFree(head) = TRUE;
	int* next_in_memory = (int*)NULL;
	if ((int*)((char*)head + *head) <= (int*)((char*)pMemory1 + list.size)) { ///???????
		next_in_memory = (int*)((char*)head + *head);
	}
	int* prev_in_memory = (int*)NULL;
	if ((int*)((char*)head - sizeof(int)) >= (int*)pMemory1) {
		int size = *(int*)((char*)head - sizeof(int));
		prev_in_memory = (int*)((char*)head - size);
	}

	if (prev_in_memory != NULL && *isFree(prev_in_memory) == TRUE) { // if prev in memory is free, we need to merge to escape fragmentation
		*prev_in_memory = *prev_in_memory + *head; // enlarge size
		*sizeCopy(prev_in_memory) = *prev_in_memory;
		head = prev_in_memory;
		merged_w_left = TRUE;

	}

	if (next_in_memory != NULL && *isFree(next_in_memory) == TRUE) { // if next in memory is free, we need to merge to escape fragmentation
		merged_w_right = TRUE;

		if (!merged_w_left) {
			*nextOf(head) = list.ptr;
			*prevOf(list.ptr) = head;
			*prevOf(head) = (int*)NULL;
			list.ptr = head;
		}

		if (*prevOf(next_in_memory))
			*nextOf(*prevOf(next_in_memory)) = *nextOf(next_in_memory);
		if (*nextOf(next_in_memory))
			*prevOf(*nextOf(next_in_memory)) = *prevOf(next_in_memory);
		if (list.ptr == next_in_memory)
			list.ptr = *nextOf(list.ptr);
		*head += *next_in_memory;
		*sizeCopy(head) = *head;
	}

	if (!merged_w_left && !merged_w_right) {
		*nextOf(head) = list.ptr;
		if (list.ptr != NULL) {
			*prevOf(list.ptr) = head;
		}
		*prevOf(head) = NULL;
		list.ptr = head;
	}
}


void memdone() {
	int* head = (int*)pMemory1;
	while ((char*)head < (char*)pMemory1 + list.size) {
		if (*isFree(head) == FALSE) {
			fprintf(stderr, "Memory leak was detected at %p!\n", (char*)head + DESC_SIZE - sizeof(int));
		}
		head = (int*)((char*)head + *head);
	}
}

int memgetminimumsize() {
	return DESC_SIZE;
}

int memgetblocksize() {
	return DESC_SIZE;
}