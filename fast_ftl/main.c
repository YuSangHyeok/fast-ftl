#include <stdio.h>
#include "main.h"
#include "filerw.h"

FILE* f = NULL;
int count_update_read = 0;
int count_read = 0;
int count_write = 0;

void main() {

	fopen_s(&f, FILE_NAME, "r+");

	if (initialize(N_BLOCK * N_SECTOR * B_SECTOR) == 1) {
		printf("Fail to initialize.");
		return;
	}

	mapping_hybrid_initialize();
	log_initialize();
	initialize_log_index();

	///////////////////////////////////////////////////////////////////
	/////////////////////// common read ///////////////////////////////
	///////////////////////////////////////////////////////////////////
	// initialize read, write count.
	count_read = 0;
	count_write = 0;
	char* r = (char*)malloc(sizeof(char)*B_DATA);
	
	// read all sectors in data area
	for (int i = 0; i < 1600; i++) {
		int k = fast_read(i/N_SECTOR, i%N_SECTOR, r, READ_DATA);
		if (k != 0) {
			printf("Can't read file: %d\n", k);
			return;
		}
	}

	// print results
	printf("\n************* common read *************\n");
	printf("Read count  : %7d times.\n", count_read);
	printf("Write count : %7d times.\n", count_write);

	///////////////////////////////////////////////////////////////////
	////////////////////// improved read //////////////////////////////
	///////////////////////////////////////////////////////////////////
	// initialize read, write count.
	count_read = 0;
	count_write = 0;

	// read all sectors in data area
	for (int i = 0; i < 1600; i++) {
		int k = fast_read_improved(i / N_SECTOR, i%N_SECTOR, r, READ_DATA);
		if (k != 0) {
			printf("Can't read file: %d\n", k);
			return;
		}
	}

	// print results
	printf("\n************ improved read ************\n");
	printf("Read count  : %7d times.\n", count_read);
	printf("Write count : %7d times.\n", count_write);

	// free memory & file stream
	free(r);
	fclose(f);
	return;
}
