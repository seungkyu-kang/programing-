// 2023-2 �������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("�÷��̾� ��: ");
	scanf_s("%d", &n_player);

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i] = true;
	}
	return 0;
}

void intro(void) {
	// "GOOD LUCK"�� ��Ÿ���� ASCII ��Ʈ ���
	printf("  ____  ___   ___  ____    _    _   _  ____ _  __\n");
	printf(" / ___|/ _ \\ / _ \\|  _ \\  | |  | | | |/ ___| |/ /\n");
	printf("| |  _| | | | | | | | | | | |  | | | | |   | ' /\n");
	printf("| |_| | |_| | |_| | |_| | | |__| |_| | |___| . \n");
	printf(" \\____|\\___/ \\___/|____/  |_____\\___/ \\____|_|\\_\\ \n");


	// ���� ������ ���� ��� �ð� (5��)
	printf("\n������ �����մϴ�...\n");
	Sleep(4800); //  (5��) ���

	// �ܼ� ȭ���� ����� ���� (Windows ȯ��)
	system("cls");
}

int main(void) {
	jjuggumi_init();
	intro();
	sample();
	//mugunghwa();
	//nightgame();
	//juldarigi();
	//jebi();
	return 0;
}