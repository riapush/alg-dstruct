#pragma once
#ifndef _MY_H 
#define _MY_H
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct aa {
		int data;
		int lvl;
		struct aa* l;
		struct aa* r;

	}aa;

	static aa* bottom = NULL;
	static aa* deleted = NULL;
	static aa* last = NULL;

	void treeInnit(void);
	aa* createNode(int data, int lvl, aa* l, aa* r);
	aa* skew(aa* tree);
	aa* split(aa* tree);
	aa* insertTree(int data, aa* tree);
	aa* removeFromTree(int data, aa* tree);
	int searchTree(int data, aa* tree);
	void destroy(aa* tree);
	int solution(FILE* stream_in, FILE* stream_out); 
#ifdef __cplusplus
}
#endif

#endif
