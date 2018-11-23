#include "main.h"
#include "filerw.h"

int mapping_block()
{
	return 1;
}

int mapping_h_set() {
	int mb = mapping_block();
	short mapping_table[64];
	for (int i = 0; i < 64; i++) {
		mapping_table[i] = 63 - i;
	}
	write_data(mb, 0, mapping_table, WRITE_DATA);
	return 0;
}

// hybrid mapping
short mapping_hybrid(short LBN)
{
	int mb = mapping_block();

	// 2 bytes for 1 block

	char* mapping_table = (char*)malloc(sizeof(char) * B_DATA);
	read_data(mb, 0, mapping_table, READ_DATA);

	short PBN = 0;
	memcpy(&PBN, mapping_table + sizeof(char)*LBN*2 , 2);

	free(mapping_table);
	return PBN;
}

int write_hybrid(int LBN, int SN, char* data, int function) {
	int PBN = mapping_hybrid(LBN);
	int offset = SN % N_SECTOR;
	write_data(PBN, offset, data, function);

	return 0;
}

int read_hybrid(int LBN, int SN, char* buf, int function) {
	int PBN = mapping_hybrid(LBN);
	int offset = SN % N_SECTOR;
	read_data(PBN, offset, buf, function);

	return 0;
}