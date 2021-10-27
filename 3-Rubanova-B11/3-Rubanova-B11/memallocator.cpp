#include "memallocator.h"
#include <inttypes.h>
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


int* isUsed(int* element) {
	return (int*)((char*)element + sizeof(int) + 2 * sizeof(int*));
}


int* sizeCopy(int* element) {
	return ((int*)((char*)element + *element - sizeof(int)));
}


int meminit(void *pMemory, int size) {
	// descriptor consist of size = size, next = NULL, prev = NULL, is used = FALSE, sizecopy in the end
	if (size <= memgetminimumsize()) // 0-size block musn't be allocated
		return 0;
	pMemory1 = pMemory;
	list.size = size;
	list.ptr = (int*)pMemory;
	*(list.ptr) = size;
	*nextOf(list.ptr) = NULL;
	*prevOf(list.ptr) = NULL;
	*isUsed(list.ptr) = FALSE;
	*sizeCopy(list.ptr) = size; // copy size in the end of block
	return OK;
}


void* memalloc(int size) {
	if (size < 1 || size > list.size - memgetblocksize())
		return NULL;
	int* head = list.ptr;
	if (head == NULL)
		return NULL;
	while (*head < (size + DESC_SIZE)) { // while suitable block not found
		if (*nextOf(head) == NULL) // if next == NULL, there's no free blocks left
			return NULL;
		head = *nextOf(head); // move to next block
	}
	// if we reached this line, we found suitable block
	// check for fragmentation
	if (*head - (size + DESC_SIZE) > DESC_SIZE) {
		// we can have 2 blocks
		int* next = (int*)((char*)head + DESC_SIZE + size);
		*next = *head - (size + DESC_SIZE); // size of next equals to size of current block without taken memory
		*sizeCopy(next) = *head - (size + DESC_SIZE); // size copy in the end
		*nextOf(next) = *nextOf(head); // next.next equals head.next
		*prevOf(next) = *prevOf(head); // next.prev equals head.prev
		*isUsed(next) = FALSE;
		if (*prevOf(head) != NULL) // if head had prev element
			*nextOf(*prevOf(head)) = next;
		else
			list.ptr = next;
		if (*nextOf(head) != NULL)
			*prevOf(*nextOf(head)) = next;
		*head = size + DESC_SIZE;
		*sizeCopy(head) = *head; // size copy in the end

	}
	else { // then we have fragmentation. to escape that situation we don't divide block
		// head.size doesn't change
		// just delete head out of list
		int* next = *nextOf(head);
		int* prev = *prevOf(head);
		if (next == NULL && prev == NULL) {
			list.ptr = NULL; // list of free blocks is empty 
		}
		else if (prev == NULL) {
			*prevOf(next) = NULL; // then next1 becomes first
			list.ptr = next;
		}
		else if (next == NULL) {
			*nextOf(prev) = NULL; // if next == null, then prev becomes last
		}
		else {
			*nextOf(prev) = next;
			*prevOf(next) = prev;
		}
	}
	*isUsed(head) = TRUE; // is used
	*nextOf(head) = NULL; // head is occupied so it's out of list
	*prevOf(head) = NULL;
	return (void*)((char*)head + DESC_SIZE - sizeof(int)); // first cells are occupied w/ descriptor
}


void memfree(void *p) {
	if (p == NULL) {
		return;
	}
	int* head = (int*)((char*)p - DESC_SIZE + sizeof(int)); // go to descriptor
	*isUsed(head) = FALSE;
	int merged_w_left = FALSE;
	int merged_w_right = FALSE;
	int* next_in_memory = NULL;
	int* prev_in_memory = NULL;
	if ((int*)((char*)head + *head) <= (int*)((char*)pMemory1 + list.size)) {
		next_in_memory = (int*)((char*)head + *head);
	}
	if ((int*)((char*)head - sizeof(int)) >= (int*)pMemory1) {
		int size = *(int*)((char*)head - sizeof(int));
		prev_in_memory = (int*)((char*)head - size);
	}

	if (prev_in_memory != NULL && *isUsed(prev_in_memory) == FALSE) { // if prev in memory is free, we need to merge to escape fragmentation
		merged_w_left = TRUE;

		*prev_in_memory = *prev_in_memory + *head; // enlarge size
		*sizeCopy(prev_in_memory) = *prev_in_memory;
		head = prev_in_memory;

	}

	if (next_in_memory != NULL && *isUsed(next_in_memory) == FALSE) { // if next in memory is free, we need to merge to escape fragmentation
		merged_w_right = TRUE;

		if (merged_w_left == FALSE) {
			*nextOf(head) = list.ptr;
			*prevOf(list.ptr) = head;
			*prevOf(head) = NULL;
			list.ptr = head;
		}
		
		if (next_in_memory == list.ptr) {
			list.ptr = *nextOf(next_in_memory);
			*prevOf(list.ptr) = NULL;
		}
		else {
			*nextOf(*prevOf(next_in_memory)) = *nextOf(next_in_memory);
			if (*nextOf(next_in_memory) != NULL)
				*prevOf(*nextOf(next_in_memory)) = *prevOf(next_in_memory);
		}
		*head += *next_in_memory;
		*sizeCopy(head) = *head;
	}

	if (merged_w_left == FALSE && merged_w_right == FALSE) {
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
	while ((char*)head < ((char*)pMemory1 + list.size)) {
		if (*isUsed(head) == TRUE) {
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