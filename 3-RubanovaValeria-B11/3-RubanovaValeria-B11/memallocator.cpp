#include "memallocator.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#define OK 1
#define ERROR -1
#define FALSE (uint32_t)0
#define TRUE (uint32_t)1
#define DESC_SIZE (3 * sizeof(uint32_t) + 2 * sizeof(uint32_t*)) // descriptor size


typedef struct init {
	uint32_t* ptr;
	uint32_t size;

}init;


init list;
void *pMemory1;


uint32_t** prevOf(uint32_t* element) {
	return (uint32_t**)((char*)element + sizeof(uint32_t) + sizeof(uint32_t*));
}


uint32_t** nextOf(uint32_t* element) {
	return (uint32_t**)((char*)element + sizeof(uint32_t));
}


uint32_t* flagOf(uint32_t* element) {
	return (uint32_t*)((char*)element + sizeof(uint32_t) + 2 * sizeof(uint32_t*));
}


uint32_t* sizeCopy(uint32_t* element) {
	return (uint32_t*)((char*)element + *element - sizeof(uint32_t));
}


int meminit(void *pMemory, int size) {
	// descriptor consist of size = size, next = NULL, prev = NULL, is used = FALSE, sizecopy in the end
	if (size <= memgetminimumsize()) // 0-size block musn't be allocated
		return ERROR;
	pMemory1 = pMemory;
	list.size = size;
	list.ptr = (uint32_t*)pMemory;
	*(list.ptr) = size;
	*nextOf(list.ptr) = (uint32_t*)NULL; // next 
	*prevOf(list.ptr) = (uint32_t*)NULL; // prev
	*flagOf(list.ptr) = FALSE; // is used
	*sizeCopy(list.ptr) = size; // copy size in the end of block
	return OK;
}


void* memalloc(int size) {
	if ((uint32_t)size < 1 || (uint32_t)size > list.size - memgetblocksize())
		return NULL;
	uint32_t* head = list.ptr;
	if (head == (uint32_t*)NULL) {
		return NULL;
	}
	while (*head < (size + DESC_SIZE)) { // while suitable block not found
		if (*nextOf(head) == (uint32_t*)NULL) { // if next == NULL, there's no free blocks left
			return NULL;
		}
		head = *nextOf(head); // move to next block
	}
	// if we reached this line, we found suitable block
	// check for fragmentation
	if (*head - (size + DESC_SIZE) > DESC_SIZE) {
		// we can have 2 blocks
		uint32_t* freeBlock = (uint32_t*)((char*)head + DESC_SIZE + size);
		*freeBlock = *head - (size + DESC_SIZE); // size of next equals to size of current block without taken memory
		*sizeCopy(freeBlock) = *freeBlock; // size copy in the end
		*nextOf(freeBlock) = *nextOf(head); // next.next equals head.next
		*prevOf(freeBlock) = *prevOf(head); // next.prev equals head.prev
		*flagOf(freeBlock) = FALSE; // is used
		if (*prevOf(head) != (uint32_t*)NULL) { // if head had prev element
			uint32_t* prev = *prevOf(head); //we need to change prev.next from head to next
			*nextOf(prev) = freeBlock;
		}
		else {
			list.ptr = freeBlock;
		}
		if (*nextOf(head) != (uint32_t*)NULL) {
			uint32_t* next1 = *nextOf(head); //we need to change next.next from head to prev
			*prevOf(next1) = freeBlock;
		}
		*head = size + DESC_SIZE;
		*sizeCopy(head) = *head; // size copy in the end

	}
	else { // then we have fragmentation. to escape that situation we don't divide block
		// head.size doesn't change
		// just delete head out of list
		uint32_t* next = *nextOf(head);
		uint32_t* prev = *prevOf(head);
		if (next == (uint32_t*)NULL && prev == (uint32_t*)NULL) {
			list.ptr = NULL; // list of free blocks is empty 
		}
		else if (prev == (uint32_t*)NULL) {
			*prevOf(next) = (uint32_t*)NULL; // then next1 becomes first
			list.ptr = next;
		}
		else if (next == (uint32_t*)NULL) {
			*nextOf(prev) = (uint32_t*)NULL; // if next == null, then prev becomes last
		}
		else {
			*nextOf(prev) = next;
			*prevOf(next) = prev;
		}
	}
	*flagOf(head) = TRUE; // is used
	*nextOf(head) = (uint32_t*)NULL; // head is occupied so it's out of list
	*prevOf(head) = (uint32_t*)NULL;
	return (void*)((char*)head + DESC_SIZE - sizeof(uint32_t)); // first 4 cells are occupied w/ descriptor
}


void memfree(void *p) {
	if (p == NULL) {
		return;
	}
	uint32_t merged_w_left = FALSE;
	uint32_t merged_w_right = FALSE;
	uint32_t* head = (uint32_t*)((char*)p - DESC_SIZE + sizeof(uint32_t)); // go to descriptor
	*flagOf(head) = FALSE;
	uint32_t* next_in_memory = (uint32_t*)NULL;
	if ((uint32_t*)((char*)head + *head) <= (uint32_t*)((char*)pMemory1 + list.size)) { ///???????
		next_in_memory = (uint32_t*)((char*)head + *head);
	}
	uint32_t* prev_in_memory = (uint32_t*)NULL;
	if ((uint32_t*)((char*)head - sizeof(uint32_t)) >= (uint32_t*)pMemory1) {
		uint32_t size = *(uint32_t*)((char*)head - sizeof(uint32_t));
		prev_in_memory = (uint32_t*)((char*)head - size);
	}

	if (prev_in_memory != NULL && *flagOf(prev_in_memory) == FALSE) { // if prev in memory is free, we need to merge to escape fragmentation
		*prev_in_memory = *prev_in_memory + *head; // enlarge size
		*sizeCopy(prev_in_memory) = *prev_in_memory;
		head = prev_in_memory;
		merged_w_left = TRUE;

	}

	if (next_in_memory != NULL && *flagOf(next_in_memory) == FALSE) { // if next in memory is free, we need to merge to escape fragmentation
		merged_w_right = TRUE;

		if (!merged_w_left) {
			*nextOf(head) = list.ptr;
			*prevOf(list.ptr) = head;
			*prevOf(head) = (uint32_t*)NULL;
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
	uint32_t* head = (uint32_t*)pMemory1;
	while ((char*)head < (char*)pMemory1 + list.size) {
		if (*flagOf(head) == TRUE) {
			fprintf(stderr, "Memory leak was detected at %p!\n", (char*)head + DESC_SIZE - sizeof(uint32_t));
		}
		head = (uint32_t*)((char*)head + *head);
	}
}

int memgetminimumsize() {
	return DESC_SIZE;
}

int memgetblocksize() {
	return DESC_SIZE;
}