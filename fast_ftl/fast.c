#include "main.h"
#include "filerw.h"
#include "log.h"

int log_initialize() {
	for (int log_block = 3; log_block < 6; log_block++) {
		for (int sector = 0; sector < 32; sector++) {
			SECTOR log;
			log.data = (char*)malloc(sizeof(char) * B_DATA);
			log.spare = (char*)malloc(sizeof(char) * B_SPARE);
			sprintf_s(log.data, sizeof(char)*B_DATA, "Data is updated in log_block %d, sector %d.", log_block, sector);
			
			LOG_SPARE log_spare;
			log_spare.used = 1;
			log_spare.LPN = rand() % 50 * N_SECTOR;
			memcpy(log.spare, &log_spare, sizeof(B_SPARE));

			write_data(log_block, sector, &log, WRITE_ALL | IGNORE_USE);
			free(log.data);
			free(log.spare);
		}
	}
}

int log_mapping_block()
{
	return 2;
}


int fast_write() {

}

int fast_read(int LBN, int SN, char* buf, int function) {

	int n_log_mapping_block = log_mapping_block();
	int LPN = LBN * N_SECTOR + SN;

	SECTOR log;

	log.data = (char*)malloc(sizeof(char) * B_DATA);
	log.spare = (char*)malloc(sizeof(char) * B_SPARE);


	for (int log_block = 5; log_block >= 3; log_block--) {
		for (int sector = 31; sector >= 0; sector--) {
			read_data(log_block, sector, &log, READ_ALL);
			LOG_SPARE log_spare;
			memcpy(&log_spare, log.spare, sizeof(LOG_SPARE));

			if (log_spare.used == 1 && log_spare.LPN == LPN) {
				switch (function) {
					case READ_DATA:
						memcpy(buf, log.data, B_DATA);
						break;
					case READ_SPARE:
						memcpy(buf, log.spare, B_SPARE);
						break;
					case READ_ALL:
						memcpy(buf, &log, B_SECTOR);
						break;
				}
				free(log.data);
				free(log.spare);
				return 0;
			}
		}
	}
	
	// Read from Data Block
	read_hybrid(LBN, SN, buf, function);
	free(log.data);
	free(log.spare);
	return 0;
}
