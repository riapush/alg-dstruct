#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#pragma warning(disable:4996)


typedef struct {
	char* str;
	int deleted;
} node;

node* map;

const int size = 524288;

void hashMap(void) {
	map = (node*)malloc(size * sizeof(node));
	if (!map) return;
	for (int i = 0; i < size; i++) {
		map[i].str = NULL;
		map[i].deleted = 0;
	}
}

int hash(const char *str, int iter) {
	int c0 = 17;
	double c1 = 0.5;
	double c2 = 0.5;
	int h1 = 0;
	for (int i = 0; str[i] != '\0'; i++)
		h1 = c0 * h1 + str[i];
	h1 = h1 % size;

	int h2 = (int)(h1 + c1 * iter + c2 * iter*iter) % size;

	return h2;
}

int search(const char* str) {
	int h = hash(str, 0);
	int i = 0;
	while (map[h].str && i < size) {
		if (!map[h].deleted && strcmp(map[h].str, str) == 0) {
			return 1;
		}
		i++;
		h = hash(str, i);
	}
	return 0;
}

void insert(const char* str) {
	int i = 0;
	int h = hash(str, 0);
	int first_deleted = -1;
	while (map[h].str && i < size) {
		if (strcmp(map[h].str, str) == 0 && !map[h].deleted) {
			return;
		}
		if (map[h].deleted && first_deleted == -1) {
			first_deleted = h;
		}
		i++;
		h = hash(str, i);
	}

	if (first_deleted != -1) {
		map[first_deleted].str = (char*)malloc((strlen(str) + 1) * sizeof(char));
		strcpy(map[first_deleted].str, str);
		map[first_deleted].deleted = 0;
	}
	else {
		map[h].str = (char*)malloc((strlen(str) + 1) * sizeof(char));
		strcpy(map[h].str, str);
	}
}

void removeElement(const char* str) {
	int h = hash(str, 0);
	int i = 0;
	while (map[h].str && i < size) {
		if (strcmp(map[h].str, str) == 0 && !map[h].deleted) {
			map[h].deleted = 1;
			return;
		}
		i++;
		h = hash(str, i);
	}
}

void destroyHashMap(void) {
	if (!map) return;
	for (int i = 0; i < size; i++)
		free(map[i].str);
	free(map);
	map = NULL;
}

int main() {
	hashMap();
	char action;
	char str[10000];
	while (scanf("%c", &action) > 0) {
		switch (action) {
		case 'a':
			scanf("%s", &str);
			insert(str);
			break;
		case 'f':
			scanf("%s", &str);
			if (search(str))
				printf("%s", "yes\n");
			else
				printf("%s", "no\n");
			break;
		case 'r':
			scanf("%s", &str);
			removeElement(str);
			break;
		}
	}
	destroyHashMap();

	return 0;
}