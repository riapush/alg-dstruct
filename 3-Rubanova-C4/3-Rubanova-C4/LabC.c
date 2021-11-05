#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning(disable : 4996)
#define ERROR -1
#define OK 1


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


void init(queue* q) {
	if (q == NULL)
		return;
	q->head = NULL;
	q->tail = NULL;
}


int isEmpty(queue* q) {
	if (q == NULL)
		return 0;
	if (q->head == NULL && q->tail == NULL) {
		return 1;
	}
	return 0;
}


void insert(queue* q, int vertex) {
	if (q == NULL)
		return;

	q_list* element = (q_list*)malloc(sizeof(q_list));
	if (element == NULL)
		return;

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
}


int pop(queue* q) {

	if (q == NULL)
		return ERROR;

	if (isEmpty(q)) {
		return ERROR;
	}

	int vertex = q->head->vertex;

	q_list* tmp = q->head;
	q->head = q->head->next;
	free(tmp);

	if (q->head == NULL) {
		q->tail = NULL;
	}

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


void breadthFirstSearch(int vertex) {
	visited[vertex] = 1;
	queue* q = (queue*)malloc(sizeof(queue));
	if (q == NULL) {
		printf("Memory allocation error for q in breadthFirstSearch\n");
		return;
	}
	init(q);
	insert(q, 0);
	while (q->head != NULL) {
		int current = pop(q);
		printf("%i ", current);
		for (int y = 0; y < n; y++) {
			if (tree[current][y] == 1) {
				if (visited[y] == 1) {
					continue;
				}
				else {
					insert(q, y);
					visited[y] = 1;
				}
			}
		}
		visited[current] = 1;
	}
}


void stress_test() {
	unsigned long start = clock();
	int num_v = 15000;
	free(tree);
	tree = (int**)malloc(sizeof(int*) * num_v);
	if (tree == NULL) {
		printf("Memory allocation error for tree in stress_test\n");
		return;
	}
	for (int i = 0; i < num_v; i++) {
		tree[i] = (int*)malloc(sizeof(int) * num_v);
		if (tree[i] == NULL) {
			for (int j = i - 1; j >= 0; j--) {
				free(tree[j]);
			}
			free(tree);
			printf("Memory allocation error for tree[%i] in stress_test\n", i);
			return;
		}
		for (int j = 0; j < num_v; j++) {
			tree[i][j] = rand() % 2;
		}
	}

	visited = (int*)malloc(sizeof(int) * num_v);
	if (visited == NULL) {
		printf("Memore allocation error for visited in stess_test\n");
		for (int i = 0; i < n; i++) {
			free(tree[i]);
		}
		free(tree);
		return;
	}
	for (int i = 0; i < num_v; i++) {
		visited[i] = 0;
	}

	breadthFirstSearch(0);
	double time = (double)(clock() - start) / CLOCKS_PER_SEC;
	printf("%f\n", time);
	for (int i = 0; i < n; i++) {
		free(tree[i]);
	}
	free(tree);
}


int main(void) {
	stress_test();
	/*fscanf(stdin, "%d", &n);

	tree = (int**)malloc(sizeof(int*) * n);
	if (tree == NULL) {
		printf("Memory allocation error for tree in main\n");
		return 1;
	}

	for (int i = 0; i < n; i++) {
		tree[i] = (int*)malloc(sizeof(int) * n);
		if (tree[i] == NULL) {
			for (int j = i - 1; j >= 0; j--) {
				free(tree[j]);
			}
			printf("Memory allocation error for tree[%i] in main\n", i);
			return 1;
		}
	}
	visited = (int*)malloc(sizeof(int) * n);
	if (visited == NULL) {
		for (int i = 0; i < n; i++) {
			free(tree[i]);
		}
		free(tree);
		printf("Memore allocation error for visited in main\n");
		return 1;
	}
	for (int i = 0; i < n; i++)
		visited[i] = 0;

	int x, y;
	while (fscanf(stdin, "%d %d", &x, &y) != EOF) {
		tree[x][y] = 1;
		tree[y][x] = 1;
	}

	breadthFirstSearch(0);
	for (int i = 0; i < n; i++) {
		free(tree[i]);
	}
	free(tree);
	free(visited);
	*/


}