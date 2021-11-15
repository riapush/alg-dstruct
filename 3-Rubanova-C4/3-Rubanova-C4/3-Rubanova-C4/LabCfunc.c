#include <stdio.h>
#include <stdlib.h>
#include "LabC.h"
#pragma warning(disable : 4996)
#define ERROR -1
#define OK 1


void init(queue* q) {
	if (q == NULL)
		return;

	q->head = NULL;
	q->tail = NULL;
}


int isEmpty(queue* q) {
	if (q == NULL)
		return ERROR;

	if (q->head == NULL && q->tail == NULL)
		return 1;
	return 0;
}


int insert(queue* q, int vertex) {
	if (q == NULL)
		return ERROR;

	q_list* element = (q_list*)malloc(sizeof(q_list));
	if (element == NULL)
		return ERROR;

	element->vertex = vertex;
	element->next = NULL;

	if (isEmpty(q)) {
		q->head = element;
		q->tail = q->head;
	}
	else {
		q->tail->next = element;
		q->tail = q->tail->next;
	}

	return OK;
}


int pop(queue* q) {

	if (q == NULL)
		return ERROR;

	if (isEmpty(q))
		return ERROR;

	int vertex = q->head->vertex;

	q_list* tmp = q->head;
	q->head = q->head->next;
	free(tmp);

	if (q->head == NULL)
		q->tail = NULL;

	return vertex;
}


void freeQueue(queue* q) {
	if (q == NULL)
		return;
	while (!isEmpty(q)) {
		int i = pop(q);
	}
	free(q);
}


int breadthFirstSearch(int vertex) {
	visited[vertex] = 1;
	queue* q = (queue*)malloc(sizeof(queue));
	if (q == NULL) {
		printf("Memory allocation error for q in breadthFirstSearch\n");
		return ERROR;
	}
	init(q);
	if (insert(q, 0) == ERROR)
		return ERROR;
	while (q->head != NULL) {
		int current = pop(q);
		printf("%i ", current);
		for (int y = 0; y < n; y++) {
			if (tree[current][y] == 1) {
				if (visited[y] == 1)
					continue;
				else {
					if (insert(q, y) == ERROR) {
						freeQueue(q);
						return ERROR;
					}
					visited[y] = 1;
				}
			}
		}
		visited[current] = 1;
	}
	free(q);
	return OK;
}