#include <stdio.h>
#include "main.h"
#include "filerw.h"

void main() {

	if (initialize(N_BLOCK * N_SECTOR * B_SECTOR) == 1) {
		printf("Fail to initialize.");
		return;
	}

	mapping_h_set();

	char* r = (char*)malloc(sizeof(char)*B_SECTOR);
	char* w = (char*)malloc(sizeof(char)*B_DATA);
	sprintf_s(w, sizeof(char)*B_DATA, "123123");

	write_hybrid(3, 6, w, WRITE_DATA);
	int k = read_hybrid(3, 6, r, READ_ALL);
	if (k != 0) {
		printf("Can't read file: %d\n", k);
		return;
	}
	printf("%s\n", r);

	free(w);
	free(r);
	return;
}
