#include <stdio.h>
#include "main.h"
#include "filerw.h"

extern FILE* f;
extern int count_read;
extern int count_write;

// initialize clear flash memory (all 0)
int initialize(int memSize) {
	fseek(f, 0, SEEK_SET);

	if (f == NULL) {
		return 1;
	}

	printf("memSize : %d\n", memSize);

	int sectorNum = memSize / B_SECTOR;

	char* buf = (char*)malloc(sizeof(char) * B_SECTOR);
	memset(buf, 0, sizeof(char)*B_SECTOR);

	for (int i = 0; i < sectorNum; i++) {
		fwrite(buf, sizeof(char)*B_SECTOR, 1, f);
	}

	free(buf);
	return 0;
}

/***** return information ********
 * return 0 : successfuly readed
 * return -1 : invalid function
 * return -2 : data is already exist (use IGNORE_USE flag)
 */
int write_data(int blockNum, int offset, char *data, int function) {
	// count write
	count_write++;

	// distinct flag
	int write_function = function & 0x0F;
	int ignore_function = function & 0xF0;

	// used check
	if (ignore_function != IGNORE_USE) {
		int used = used_check(blockNum, offset);
		if (used == 1) {
			printf("Block %d, offset %d is already used!!\n", blockNum, offset);
			return -2;
		}
	}

	// fwriter function
	int pointer;
	switch (write_function) {
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

	fseek(f, 0, SEEK_SET);
	return 0;
}

/***** return information ********
 * return 0 : successfuly readed
 * return -1 : invalid function
 */
int read_data(int blockNum, int offset, char *buf, int function) {
	// count read;
	count_read++;

	// file is not exist or already opened.
	if (f == NULL) {
		printf("File is not exist or already opened.\n");
		return -2;
	}

	// fread function
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

	fseek(f, 0, SEEK_SET);
	return 0;
}

int erase_data(int blockNum) {
	char* buf = (char*)malloc(sizeof(char) * B_BLOCK);
	memset(buf, 0, sizeof(char) * B_BLOCK);

	int pointer = get_pointer(blockNum, 0);
	fseek(f, pointer, SEEK_SET);
	fwrite(buf, sizeof(char)*B_BLOCK, 1, f);

	free(buf);
	fseek(f, 0, SEEK_SET);
	return 0;
}

int get_pointer(int blockNum, int offset)
{
	return (blockNum * B_BLOCK) + (offset * B_SECTOR);
}

// return 1 : is used
// return 0 : not used
int used_check(int blockNum, int offset) {
	char* spare = (char*)malloc(sizeof(char) * B_SPARE);

	read_data(blockNum, offset, spare, READ_SPARE);
	if (spare[0] != 0) {
		return 1;
	}

	return 0;
}
