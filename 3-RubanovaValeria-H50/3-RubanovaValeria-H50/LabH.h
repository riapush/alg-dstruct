#pragma once

typedef struct aa {
	int data;
	int lvl;
	struct aa* l;
	struct aa* r;

}aa;

void treeInnit(void);
aa* createNode(int data, int lvl, aa* l, aa* r);
aa* skew(aa* tree);
aa* split(aa* tree);
aa* insertTree(int data, aa* tree);
aa* removeFromTree(int data, aa* tree);
int searchTree(int data, aa* tree);
void destroy(aa* tree);
int solution(FILE* stream_in, FILE* stream_out);
