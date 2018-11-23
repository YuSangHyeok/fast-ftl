#include <stdio.h>
#include "main.h"
#include "filerw.h"

void main() {

	if (initialize(N_BLOCK * N_SECTOR * B_SECTOR) == 1) {
		printf("�ʱ�ȭ�� �����Ͽ����ϴ�.");
		return;
	}

	mapping_h_set();

	char* r = (char*)malloc(sizeof(char)*B_SECTOR);
	char* w = (char*)malloc(sizeof(char)*B_DATA);
	sprintf_s(w, sizeof(char)*B_DATA, "123123");

	write_hybrid(3, 6, w, WRITE_DATA);
	int k = read_hybrid(3, 6, r, READ_ALL);
	if (k != 0) {
		printf("������ ���� �� �����ϴ�: %d\n", k);
		return;
	}
	printf("%s\n", r);


	mapping_h(0);
	mapping_h(1);
	mapping_h(2);
	mapping_h(3);

	free(w);
	free(r);
	return;
}
