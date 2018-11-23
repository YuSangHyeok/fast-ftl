#pragma once
#define B_DATA 512		// Byte of data area
#define B_SPARE 16		// Byte of spare area
#define B_SECTOR 528	// Byte of sector (B_DATA + B_SPARE)
#define B_BLOCK 16896	// Byte of block (B_SECTOR * N_SECTOR)

#define N_SECTOR 32		// Number of sector per block
#define N_BLOCK 64		// Number of blocks in system

#define FILE_NAME "flash"

typedef struct _Sector {
	char* data;
	char* spare;
} SECTOR;