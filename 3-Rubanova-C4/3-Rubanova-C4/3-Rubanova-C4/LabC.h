#pragma once

typedef struct q_list {
	int vertex;
	struct q_list* next;
}q_list;

typedef struct queue {
	q_list* head;
	q_list* tail;
}queue;

int n;
int** tree;
int* visited;

void init(queue* q);

int isEmpty(queue* q);

int insert(queue* q, int vertex);

int pop(queue* q);

void freeQueue(queue* q);

int breadthFirstSearch(int vertex);

