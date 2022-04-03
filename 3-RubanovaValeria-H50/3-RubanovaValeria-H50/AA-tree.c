#include "LabH.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

static aa* bottom = NULL;
static aa* deleted = NULL;
static aa* last = NULL;

void treeInnit(void) {
	if (!bottom) {
		bottom = (aa*)malloc(sizeof(aa));
		if (!bottom) fprintf(stderr, "Memory allocation error!\n");
		else {
			bottom->lvl = 0;
			bottom->l = bottom;
			bottom->r = bottom;
			deleted = bottom;
		}
	}
}

aa* createNode(int data, int lvl, aa* l, aa* r) {
	aa* node = (aa*)malloc(sizeof(aa));
	if (!node) {
		fprintf(stderr, "Memory allocation error!\n");
		return NULL;
	}
	else {
		node->data = data;
		node->lvl = lvl;
		node->l = l;
		node->r = r;
	}
	return node;
}

aa* skew(aa* tree) {
	if (!tree) return NULL;
	if (!tree->l) return tree;
	if (tree->lvl == tree->l->lvl) { // swap the pointers of horizontal left links.
		aa* node = tree;
		tree = tree->l;
		node->l = tree->r;
		tree->r = node;
	}
	return tree;
}

aa* split(aa* tree) {
	if (!tree) return NULL;
	if (!tree->r || !tree->r->r) return tree;
	if (tree->lvl == tree->r->r->lvl) { // we have two horizontal right links.  Take the middle node, elevate it, and return it.
		aa* node = tree;
		tree = tree->r;
		node->r = tree->l;
		tree->l = node;
		++tree->lvl;
	}
	return tree;
}

aa* insertTree(int data, aa* tree) {
	if (tree == bottom || !tree) {
		tree = createNode(data, 1, bottom, bottom);
		if (!tree) return NULL;
	}
	if (data < tree->data) tree->l = insertTree(data, tree->l);
	else if (data > tree->data) tree->r = insertTree(data, tree->r);
	tree = skew(tree);
	tree = split(tree);
	return tree;
}

aa* removeFromTree(int data, aa* tree) {
	if (tree != bottom && tree) {
		last = tree;
		if (data < tree->data) tree->l = removeFromTree(data, tree->l);
		else {
			deleted = tree;
			tree->r = removeFromTree(data, tree->r);
		}
		if (tree == last && deleted != bottom && data == deleted->data) {
			deleted->data = tree->data;
			deleted = bottom;
			tree = tree->r;
			free(last);
		}
		else if ((tree->l->lvl < tree->lvl - 1) || (tree->r->lvl < tree->lvl - 1)) {
			--tree->lvl;
			if (tree->r->lvl > tree->lvl) tree->r->lvl = tree->lvl;
			tree = skew(tree);
			tree->r = skew(tree->r);
			tree->r->r = skew(tree->r->r);
			tree = split(tree);
			tree->r = split(tree->r);
		}
	}
	return tree;
}

int searchTree(int data, aa* tree) {
	aa* node = tree;
	while (node) {
		if (node == bottom) return 0;
		if (node->data == data) return 1;
		else if (node->data < data) node = node->r;
		else node = node->l;
	}
	return 0;
}

void destroy(aa* tree) {
	if (tree == bottom || !tree) return;
	destroy(tree->l);
	destroy(tree->r);
	free(tree);
}