#pragma once
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


