#include "main.h"
#include "filerw.h"

typedef struct _index {
	int LPN;
	int block;
	int offset;
	struct INDEX* left;
	struct INDEX* right;
} INDEX;

INDEX* root = NULL;


void index_add(INDEX index) {
	INDEX* temp = root;

	while (temp != NULL) {
		if (index.LPN > temp->LPN) { // index LPN is bigger than temp
			temp = temp->right;
		}
		else if (index.LPN < temp->LPN) { // index LPN is smaller than temp
			temp = temp->left;
		}
		else {
			return; // duplicated LPN
		}
	}

	temp = (INDEX*)malloc(sizeof(INDEX));
	temp->LPN = index.LPN;
	temp->block = index.block;
	temp->offset = index.offset;
	return;
}

/***** return information ********
 * return INDEX* : Successfuly finded
 * return NULL   : No log matches the LPN
 */
INDEX* index_search(int LPN) {
	INDEX* temp = root;

	while (temp != NULL) {
		if (LPN > temp->LPN) { // index LPN is bigger than temp
			temp = temp->right;
		}
		else if (LPN < temp->LPN) { // index LPN is smaller than temp
			temp = temp->left;
		}
		else {
			return temp; // same LPN
		}
	}

	// LPN is not found
	return NULL;
}

// make log index
int initialize_log_index() {
	// search updated data from log block
	char* spare = (char*)malloc(sizeof(char) * B_SPARE);
	for (int log_block = 5; log_block >= 3; log_block--) {
		for (int sector = 31; sector >= 0; sector--) {
			read_data(log_block, sector, spare, READ_SPARE);
			
			char used;
			int log_LPN;
			memcpy(&used, spare, sizeof(char));
			memcpy(&log_LPN, spare + sizeof(char), sizeof(int));

			if (used == 1) {
				INDEX index;
				index.LPN = log_LPN;
				index.block = log_block;
				index.offset = sector;
				index.left = NULL;
				index.right = NULL;
				index_add(index);
			}
		}
	}
	free(spare);
	return 0;
}

// improved FAST FTL read function
int fast_read_improved(int LBN, int SN, char* buf, int function) {

	int n_log_mapping_block = log_mapping_block();
	int LPN = LBN * N_SECTOR + SN;

	// search updated data from log block index
	INDEX* index = index_search(LPN);

	// No log finds the LPN
	if (index == NULL) {
		// Read from Data Block
		read_hybrid(LBN, SN, buf, function);
	}
	// Log finds the LPN
	else {
		read_data(index->block, index->offset, buf, function);
	}

	return 0;
}
