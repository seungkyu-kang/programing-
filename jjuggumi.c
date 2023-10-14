// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

int jjuggumi_init(void);

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	printf("플레이어 수: ");
	scanf_s("%d", &n_player);

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i] = true;
	}
	return 0;
}

void intro(void) {
	// "GOOD LUCK"을 나타내는 ASCII 아트 출력
	printf("  ____  ___   ___  ____    _    _   _  ____ _  __\n");
	printf(" / ___|/ _ \\ / _ \\|  _ \\  | |  | | | |/ ___| |/ /\n");
	printf("| |  _| | | | | | | | | | | |  | | | | |   | ' /\n");
	printf("| |_| | |_| | |_| | |_| | | |__| |_| | |___| . \n");
	printf(" \\____|\\___/ \\___/|____/  |_____\\___/ \\____|_|\\_\\ \n");


	// 게임 시작을 위한 대기 시간 (5초)
	printf("\n게임을 시작합니다...\n");
	Sleep(4800); //  (5초) 대기

	// 콘솔 화면을 지우는 명령 (Windows 환경)
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
