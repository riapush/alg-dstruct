#pragma once

typedef struct aa {
	int data;
	int lvl;
	struct aa* l;
	struct aa* r;

}aa;

aa* skew(aa* tree);
aa* split(aa* tree);
aa* insert(aa* tree, const int data);
aa* removeFromTree(aa* tree, const int data);
aa* search(const aa* tree, const int data);
void destroy(aa* tree);
int solution(FILE* stream_in, FILE* stream_out);
