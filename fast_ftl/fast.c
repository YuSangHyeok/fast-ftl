#include "main.h"
#include "filerw.h"

extern int count_update_read;

int log_initialize() {
	srand((unsigned)time(NULL));
	for (int log_block = 3; log_block < 6; log_block++) {
		for (int sector = 0; sector < 32; sector++) {
			char* log = (char*)malloc(sizeof(char) * B_SECTOR);
			char* data = (char*)malloc(sizeof(char) * B_DATA);
			char* spare = (char*)malloc(sizeof(char) * B_SPARE);
			sprintf_s(data, sizeof(char)*B_DATA, "Data is updated in log_block %d, sector %d.", log_block, sector);
			
			char used = 1;
			int log_LPN = rand() % 50 * N_SECTOR;

			memcpy(spare, &used, sizeof(char));
			memcpy(spare + sizeof(char), &log_LPN, sizeof(int));
			memcpy(log, data, sizeof(char)*B_DATA);
			memcpy(log + sizeof(char)*B_DATA, spare, sizeof(char)*B_SPARE);
			write_data(log_block, sector, log, WRITE_ALL | IGNORE_USE);

			free(log);
			free(data);
			free(spare);
		}
	}
}

int log_mapping_block()
{
	return 2;
}


int fast_write() {

}

// common FAST FTL read funciton
int fast_read(int LBN, int SN, char* buf, int function) {

	int n_log_mapping_block = log_mapping_block();
	int LPN = LBN * N_SECTOR + SN;

	char* log = (char*)malloc(sizeof(char) * B_SECTOR);
	char* data = (char*)malloc(sizeof(char) * B_DATA);
	char* spare = (char*)malloc(sizeof(char) * B_SPARE);

	// search updated data from log block
	for (int log_block = 5; log_block >= 3; log_block--) {
		for (int sector = 31; sector >= 0; sector--) {
			read_data(log_block, sector, log, READ_ALL);
			memcpy(data, log, sizeof(char)*B_DATA);
			memcpy(spare, log + sizeof(char)*B_DATA, sizeof(char)*B_SPARE);

			char used;
			int log_LPN;
			memcpy(&used, spare, sizeof(char));
			memcpy(&log_LPN, spare + sizeof(char), sizeof(int));

			if (used == 1 && log_LPN == LPN) {
				switch (function) {
					case READ_DATA:
						memcpy(buf, data, B_DATA);
						break;
					case READ_SPARE:
						memcpy(buf, spare, B_SPARE);
						break;
					case READ_ALL:
						memcpy(buf, &log, B_SECTOR);
						break;
				}
				free(log);
				free(data);
				free(spare);
				count_update_read++;
				return 0;
			}
		}
	}
	
	// Read from Data Block
	read_hybrid(LBN, SN, buf, function);
	free(log);
	free(data);
	free(spare);
	return 0;
}

