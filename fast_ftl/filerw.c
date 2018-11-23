#include <stdio.h>
#include "main.h"
#include "filerw.h"

int initialize(int memSize) {
	FILE* f = NULL;
	fopen_s(&f, FILE_NAME, "w");

	if (f == NULL) {
		return 1;
	}

	printf("memSize : %d\n", memSize);

	int sectorNum = memSize / B_SECTOR;
	
	char* buf = (char*) malloc(sizeof(char) * B_SECTOR);
	memset(buf, 0, sizeof(char)*B_SECTOR);

	for (int i = 0; i < sectorNum; i++) {
		fwrite(buf, sizeof(char)*B_SECTOR, 1, f);
	}

	free(buf);
	fclose(f);
	return 0;
}

int write_data(int blockNum, int offset, char *data, int function) {
	FILE* f = NULL;
	fopen_s(&f, FILE_NAME, "r+");

	int pointer;
	switch (function) {
		case WRITE_DATA:
			pointer = get_pointer(blockNum, offset);
			fseek(f, pointer, SEEK_SET);
			fwrite(data, sizeof(char)*B_DATA, 1, f);
			break;
		case WRITE_SPARE:
			pointer = get_pointer(blockNum, offset) + B_DATA;
			fseek(f, pointer, SEEK_SET);
			fwrite(data, sizeof(char)*B_SPARE, 1, f);
			break;
		case WRITE_ALL:
			pointer = get_pointer(blockNum, offset);
			fseek(f, pointer, SEEK_SET);
			fwrite(data, sizeof(char)*B_SECTOR, 1, f);
			break;
		default:	// invalid function
			fclose(f);
			return -1;
	}

	fclose(f);
	return 0;
}

int read_data(int blockNum, int offset, char *buf, int function) {
	FILE* f = NULL;
	int e = fopen_s(&f, FILE_NAME, "r");

	if (f == NULL) {
		return e;
	}

	int pointer;
	switch (function) {
		case READ_DATA:
			pointer = get_pointer(blockNum, offset);
			fseek(f, pointer, SEEK_SET);
			fread_s(buf, B_DATA, B_DATA, 1, f);
			break;
		case READ_SPARE:
			pointer = get_pointer(blockNum, offset) + B_DATA;
			fseek(f, pointer, SEEK_SET);
			fread_s(buf, B_SPARE, B_SPARE, 1, f);
			break;
		case READ_ALL:
			pointer = get_pointer(blockNum, offset);
			fseek(f, pointer, SEEK_SET);
			fread_s(buf, B_SECTOR, B_SECTOR, 1, f);
			break;
		default:	// invalid function
			fclose(f);
			return -1;
	}

	fclose(f);
	return 0;
}

int erase_data(int blockNum) {
	FILE* f = NULL;
	fopen_s(&f, FILE_NAME, "r+");

	char* buf = (char*)malloc(sizeof(char) * B_BLOCK);
	memset(buf, 0, sizeof(char) * B_BLOCK);

	int pointer = get_pointer(blockNum, 0);
	fseek(f, pointer, SEEK_SET);
	fwrite(buf, sizeof(char)*B_BLOCK, 1, f);

	free(buf);
	fclose(f);
	return 0;
}

int get_pointer(int blockNum, int offset)
{
	return (blockNum * B_BLOCK) + (offset * B_SECTOR);
}

/*
void printBits(size_t const size, void const * const ptr)
{
	unsigned char *b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}
*/