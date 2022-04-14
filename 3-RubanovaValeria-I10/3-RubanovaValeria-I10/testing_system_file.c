#include <stdio.h>
#include <stdlib.h>


typedef struct node {
	struct node* parent;
	int keys[3];
	struct node* first;
	struct node* second;
	struct node* third;
	struct node* fourth;
	int size;
} node;

node* createNode(int data, node* first, node* second, node* third, node* fourth, node* parent);
int find(int data, int* keys, int size);
void swap(int* x, int* y);
void sortTwoElements(int* x, int* y);
void sortThreeElements(int* x, int* y, int* z);
void sortKeys(int size, int* keys);
void insertToNode(int data, node* tree);
void deleteKey(int data, node* tree);
void changeTo2Node(int data, node* tree, node* first, node* second);
char isLeaf(node* tree);
node* insertTree(node* tree, int value);
node* searchMin(node* tree);
node* deleteNode(node* tree, int value);
node* split(node* item);
node* searchTree(node* tree, int value);
node* fix(node* leaf);
node* merge(node* leaf);
node* redistribute(node* leaf);
void printTree(node* tree, int n);
void destroyTree(node* tree);
int solution(FILE* stream_in, FILE* stream_out);

#define BUFFER_SIZE 16

node* createNode(int data, node* first, node* second, node* third, node* fourth, node* parent) {
	node* nodeToAdd = (node*)malloc(sizeof(node));
	if (!nodeToAdd) return NULL;
	nodeToAdd->first = first;
	nodeToAdd->second = second;
	nodeToAdd->third = third;
	nodeToAdd->fourth = fourth;
	nodeToAdd->parent = parent;
	nodeToAdd->keys[0] = data;
	nodeToAdd->size = 1;
	return nodeToAdd;
}

int  find(int value, int* keys, int size) { // true if is in node, else false
	for (int i = 0; i < size; ++i)
		if (keys[i] == value) return 1;
	return 0;
}

void swap(int* a, int* b) {
	int tmp = (*a);
	(*a) = (*b);
	(*b) = tmp;
}

void sortTwoElements(int* a, int* b) {
	if (*a > *b) swap(a, b);
}

void sortThreeElements(int* a, int* b, int* c) {
	if (*a > *b) swap(a, b);
	if (*a > *c) swap(a, c);
	if (*b > *c) swap(b, c);
}

void sortKeys(int size, int* keys) {
	if (size == 1) {
		return;
	}
	if (size == 2) {
		sortTwoElements(&keys[0], &keys[1]);
	}
	if (size == 3) {
		sortThreeElements(&keys[0], &keys[1], &keys[2]);
	}
}

void insertToNode(int data, node* tree) { //insert to node, not to the tree
	if (!tree) return;

	tree->keys[tree->size] = data;
	tree->size++;
	sortKeys(tree->size, tree->keys);
}

void deleteKey(int data, node* tree) {
	if (!tree) return;

	if (tree->size >= 1 && tree->keys[0] == data) {
		tree->keys[0] = tree->keys[1];
		tree->keys[1] = tree->keys[2];
		tree->size--;
	}
	else if (tree->size == 2 && tree->keys[1] == data) {
		tree->keys[1] = tree->keys[2];
		tree->size--;
	}
}

void changeTo2Node(int data, node* tree, node* first, node* second) {
	tree->keys[0] = data;
	tree->first = first;
	tree->second = second;
	tree->third = NULL;
	tree->fourth = NULL;
	tree->parent = NULL;
	tree->size = 1;
}

char isLeaf(node* tree) {
	if (!tree) return 0;
	if (!tree->first && !tree->second && !tree->third) return 1;

	return 0;
}

node* insertTree(node* tree, int data) {
	if (!tree) return createNode(data, NULL, NULL, NULL, NULL, NULL);
	if (searchTree(tree, data)) return split(tree);
	if (isLeaf(tree)) insertToNode(data, tree);
	else if (data < tree->keys[0]) {
		if (!insertTree(tree->first, data)) return tree;
	}
	else if ((tree->size == 1) || ((tree->size == 2) && data < tree->keys[1])) {
		if (!insertTree(tree->second, data)) return tree;
	}
	else {
		if (!insertTree(tree->third, data)) return tree;
	}
	return split(tree);
}

node* searchMin(node* tree) {
	if (!tree) return tree;
	if (!tree->first) return tree;
	else return searchMin(tree->first);
}

node* deleteNode(node* tree, int data) {
	if (!tree) return NULL;

	node* deleting = searchTree(tree, data);
	if (!deleting) return tree; // if node is not there, we just return tree

	node* min = NULL;
	if (deleting->keys[0] == data) min = searchMin(deleting->second);
	else min = searchMin(deleting->third);
	if (min) {
		int* z = (data == deleting->keys[0] ? &(deleting->keys[0]) : &(deleting->keys[1]));
		swap(z, &min->keys[0]);
		deleting = min;
	}
	deleteKey(data, deleting);
	return fix(deleting);
}

node* split(node* tree) {
	if (!tree) return NULL;
	if (tree->size < 3) return tree;

	node* a = createNode(tree->keys[0], tree->first, tree->second, NULL, NULL, tree->parent);
	node* b = createNode(tree->keys[2], tree->third, tree->fourth, NULL, NULL, tree->parent);
	if (!a || !b) return NULL;

	if (a->first) a->first->parent = a;
	if (a->second) a->second->parent = a;
	if (b->first) b->first->parent = b;
	if (b->second) b->second->parent = b;

	if (tree->parent) {
		insertToNode(tree->keys[1], tree->parent);
		if (tree->parent->first == tree) {
			tree->parent->first = NULL;
		}
		else if (tree->parent->second == tree) tree->parent->second = NULL;
		else if (tree->parent->third == tree) tree->parent->third = NULL;
		if (!tree->parent->first) {
			tree->parent->fourth = tree->parent->third;
			tree->parent->third = tree->parent->second;
			tree->parent->second = b;
			tree->parent->first = a;
		}
		else if (!tree->parent->second) {
			tree->parent->fourth = tree->parent->third;
			tree->parent->third = b;
			tree->parent->second = a;
		}
		else {
			tree->parent->fourth = b;
			tree->parent->third = a;
		}
		node* tmp = tree->parent;
		free(tree);
		return tmp;
	}
	else {
		a->parent = tree;
		b->parent = tree;
		changeTo2Node(tree->keys[1], tree, a, b);
		return tree;
	}
}

node* searchTree(node* tree, int data) {
	if (!tree) return NULL;

	if (find(data, tree->keys, tree->size)) return tree;
	else if (data < tree->keys[0]) return searchTree(tree->first, data);
	else if ((tree->size == 2) && (data < tree->keys[1]) || (tree->size == 1)) return searchTree(tree->second, data);
	else if (tree->size == 2) return searchTree(tree->third, data);
}

node* fix(node* tree) { // returns 2-3 tree characteristics to the tree
	if (!tree) return NULL;

	if (tree->size == 0 && !tree->parent) {
		free(tree);
		return NULL;
	}
	if (tree->size) {
		if (tree->parent) return fix(tree->parent);
		else return tree;
	}

	node* parent = tree->parent;
	if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) tree = redistribute(tree);
	else if (parent->size == 2 && parent->third->size == 2) tree = redistribute(tree);
	else tree = merge(tree);
	return fix(tree);
}

node* merge(node* tree) {
	if (!tree) return NULL;

	node* parent = tree->parent;
	if (parent->first == tree) {
		insertToNode(parent->keys[0], parent->second);
		parent->second->third = parent->second->second;
		parent->second->second = parent->second->first;
		if (tree->first) {
			parent->second->first = tree->first;
		}
		else if (tree->second) parent->second->first = tree->second;
		if (parent->second->first) parent->second->first->parent = parent->second;
		deleteKey(parent->keys[0], parent);
		free(parent->first);
		parent->first = NULL;
	}
	else if (parent->second == tree) {
		insertToNode(parent->keys[0], parent->first);
		if (tree->first) parent->first->third = tree->first;
		else if (tree->second) parent->first->third = tree->second;
		if (parent->first->third) parent->first->third->parent = parent->first;
		deleteKey(parent->keys[0], parent);
		free(parent->second);
		parent->second = NULL;
	}
	if (!parent->parent) {
		node* tmp = NULL;
		if (parent->first) tmp = parent->first;
		else tmp = parent->second;
		tmp->parent = NULL;
		free(parent);
		return tmp;
	}
	return parent;
}

node* redistribute(node* tree) {
	if (!tree) return NULL;

	node* parent = tree->parent;
	node* first = parent->first;
	node* second = parent->second;
	node* third = parent->third;

	if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
		if (first == tree) {
			parent->first = parent->second;
			parent->second = parent->third;
			parent->third = NULL;
			insertToNode(parent->keys[0], parent->first);
			parent->first->third = parent->first->second;
			parent->first->second = parent->first->first;

			if (tree->first) parent->first->first = tree->first;
			else if (tree->second) parent->first->first = tree->second;
			if (parent->first->first) parent->first->first->parent = parent->first;
			deleteKey(parent->keys[0], parent);
		}
		else if (second == tree) {
			insertToNode(parent->keys[0], first);
			deleteKey(parent->keys[0], parent);
			if (tree->first) first->third = tree->first;
			else if (tree->second) first->third = tree->second;
			if (first->third) first->third->parent = first;
			parent->second = parent->third;
			parent->third = NULL;
			free(second);
		}
		else if (third == tree) {
			insertToNode(parent->keys[1], second);
			parent->third = NULL;
			deleteKey(parent->keys[1], parent);
			if (tree->first) second->third = tree->first;
			else if (tree->second) second->third = tree->second;

			if (second->third) second->third->parent = second;
			free(third);
		}
	}
	else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
		if (third == tree) {
			if (tree->first) {
				tree->second = tree->first;
				tree->first = NULL;
			}
			insertToNode(parent->keys[1], tree);
			if (second->size == 2) {
				parent->keys[1] = second->keys[1];
				deleteKey(second->keys[1], second);
				tree->first = second->third;
				second->third = NULL;

				if (tree->first) tree->first->parent = tree;

			}
			else if (first->size == 2) {
				parent->keys[1] = second->keys[0];
				tree->first = second->second;
				second->second = second->first;
				if (tree->first) tree->first->parent = tree;

				second->keys[0] = parent->keys[0];
				parent->keys[0] = first->keys[1];
				deleteKey(first->keys[1], first);
				second->first = first->third;
				if (second->first) second->first->parent = second;
				first->third = NULL;
			}
		}
		else if (second == tree) {
			if (third->size == 2) {
				if (!tree->first) {
					tree->first = tree->second;
					tree->second = NULL;
				}
				insertToNode(parent->keys[1], second);
				parent->keys[1] = third->keys[0];
				deleteKey(third->keys[0], third);
				second->second = third->first;
				if (second->second) second->second->parent = second;
				third->first = third->second;
				third->second = third->third;
				third->third = NULL;
			}
			else if (first->size == 2) {
				if (!tree->second) {
					tree->second = tree->first;
					tree->first = NULL;
				}
				insertToNode(parent->keys[0], second);
				parent->keys[0] = first->keys[1];
				deleteKey(first->keys[1], first);
				second->first = first->third;
				if (second->first) second->first->parent = second;
				first->third = NULL;
			}
		}
		else if (first == tree) {
			if (!tree->first) {
				tree->first = tree->second;
				tree->second = NULL;
			}
			insertToNode(parent->keys[0], first);
			if (second->size == 2) {
				parent->keys[0] = second->keys[0];
				deleteKey(second->keys[0], second);
				first->second = second->first;
				if (first->second) first->second->parent = first;
				second->first = second->second;
				second->second = second->third;
				second->third = NULL;
			}
			else if (third->size == 2) {
				parent->keys[0] = second->keys[0];
				second->keys[0] = parent->keys[1];
				parent->keys[1] = third->keys[0];
				deleteKey(third->keys[0], third);
				first->second = second->first;
				if (first->second) first->second->parent = first;
				second->first = second->second;
				second->second = third->first;
				if (second->second) second->second->parent = second;
				third->first = third->second;
				third->second = third->third;
				third->third = NULL;
			}
		}
	}
	else if (parent->size == 1) {
		insertToNode(parent->keys[0], tree);

		if (first == tree && second->size == 2) {
			parent->keys[0] = second->keys[0];
			deleteKey(second->keys[0], second);

			if (!tree->first) tree->first = tree->second;
			tree->second = second->first;
			second->first = second->second;
			second->second = second->third;
			second->third = NULL;
			if (tree->second) tree->second->parent = tree;
		}
		else if (second == tree && first->size == 2) {
			parent->keys[0] = first->keys[1];
			deleteKey(first->keys[1], first);

			if (!tree->second) tree->second = tree->first;
			tree->first = first->third;
			first->third = NULL;

			if (tree->first) tree->first->parent = tree;
		}
	}
	return parent;
}

void printTree(node* tree, int n) {
	int i;
	if (!tree) return;

	printTree(tree->first, n + 2);
	for (i = 0; i < n; i++)
		printf("  ");

	printf("%d", tree->keys[0]);
	printf("\n");
	printTree(tree->second, n + 2);
	for (int i = 0; i < n; i++)
		printf("  ");
	if (tree->size == 2) printf("%d", tree->keys[1]);
	printf("\n");
	printTree(tree->third, n + 2);
}

void destroyTree(node* tree) {
	if (!tree) return;
	destroyTree(tree->first);
	destroyTree(tree->second);
	destroyTree(tree->third);
	free(tree);
}

int solution(FILE* stream_in, FILE* stream_out) {
	char buffer[BUFFER_SIZE] = { 0 };
	char action;
	int data;
	node* tree = NULL;
	while (fgets(buffer, BUFFER_SIZE, stream_in)) {
		sscanf(buffer, "%c %i", &action, &data);
		switch (action) {
		case 'a':
			tree = insertTree(tree, data);
			break;
		case 'r':
			tree = deleteNode(tree, data);
			break;
		case 'f':
			if (searchTree(tree, data))
				fprintf(stream_out, "yes\n");
			else
				fprintf(stream_out, "no\n");
			break;
		default:
			destroyTree(tree);
			return 0;
		}
	}
	destroyTree(tree);
	return 0;
}

int main(void) {
	solution(stdin, stdout);
}