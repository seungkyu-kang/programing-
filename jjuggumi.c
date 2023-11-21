// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include "jjuggumi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1;
	}

	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];

		//id = ��ȣ
		p->id = i;

		fscanf_s(fp, "%s%d%d", p->name, (unsigned int)sizeof(p->name), &(p->intel), &(p->str));
		p->stamina = 100;

		//���� ����
		p->is_alive = true;
		p->hasitem = false;
		n_alive++;
	}

	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d", item[i].name, (unsigned int)sizeof(item[i].name), &(item[i].intel_buf), &(item[i].str_buf), &(item[i].stamina_buf));
	}
	
	fclose(fp);

	

	//n_alive = n_player;
	//for (int i = 0; i < n_player; i++) {
	//	player[i] = true;
	//}
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
	printf("\n�ε���");
	for (int i = 0; i < 5; i++) {
		printf("#########");
		Sleep(1000);
	}
	printf("\n");
	//Sleep(4800); //  (5��) ���

	// �ܼ� ȭ���� ����� ��� (Windows ȯ��)
	system("cls");
}

void ending(void) {
	int winner = -1; // ������� �ε���, �ʱ⿡�� ���º�(-1)�� ����

	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		if (p->is_alive) {
			if (winner == -1) {
				winner = i; // ó������ ��Ƴ��� �÷��̾ ����ڷ� ����
			}
			else {
				winner = -1; // �ٸ� �÷��̾ �̹� ��Ƴ��� ������ ���ºη� ����
				break;
			}
		}
	}
	if (winner == -1) {
		gotoxy(5, 6);
		printf("����ڸ� ������ ���߽��ϴ�!\n");
	}
	else {
		gotoxy(5, 6);
		printf("�÷��̾� %d�� �̰���ϴ�!\n", winner);
	}

	for (int i = 0; i < 20; i++) {
		printf("\n");
	}
	// ���� ���� ���� �߰�
	// ���� ������ �����ϰų� �ٽ� ������ �� �ֵ��� �ʿ��� �ڵ带 ���⿡ �߰�
}

int main(void) {

	jjuggumi_init();
	//intro();
	//sample();
	//mugunghwa();
	nightgame();
	//juldarigi();
	ending();
	//jebi();

	return 0;
}
