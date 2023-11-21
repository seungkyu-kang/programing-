#include "jjuggumi.h"
#include "canvas.h"



void draw(void);
void print_status(void);

// (zero-base) row��, col���� Ŀ�� �̵�
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row��, col���� ch ���
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// ���Թ� �̷��� �� �� �ִµ� �Ϻη� �� ����������
		back_buf[i][0] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '*' : ' ';
		}
	}
}

// back_buf[row][col]�� �̵��� �� �ִ� �ڸ����� Ȯ���ϴ� �Լ�
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// ��ܿ� ����, �ϴܿ��� ���� ���¸� ���
void display(void) {
	draw();

	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
	print_status();
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void print_status(void) {
	printf("no. of players left: %d\n", n_alive);
	printf("			intl	str	stm\n");
	for (int i = 0; i < n_player; i++) {
		PLAYER* p = &player[i];
		printf("player %2d: %5s", i, p->is_alive ? "alive" : "DEAD");	

		//item buf
		printf("	%2d(+%d)	%2d(+%d)	%3d%%\n", p->intel, p->item.intel_buf, p->str, p->item.str_buf, p->stamina);
	}
}

//void dialog(char message[]) {
//	int remaining_time = DIALOG_DURATION_SEC;
//	while (remaining_time >= 0) {
//		// ���� �ʰ� ���¸� ���
//		char backup[20][10];
//		for (int row = 0; row < N_ROW; row++) {
//			for (int col = 0; col < N_COL; col++) {
//				backup[row][col] = back_buf[row][col];
//			}
//		}
//
//		// ���� �ð��� �޽��� ���
//		gotoxy(N_ROW / 2, N_COL / 2 - strlen(message) / 2);
//		printf("Remaining time: %d seconds", remaining_time);
//		gotoxy(N_ROW / 2 + 1, N_COL / 2 - strlen(message) / 2);
//		printf("%s", message);
//
//		// 1�� ���
//		Sleep(1000);
//		remaining_time--;
//
//		// ����
//		for (int row = 0; row < N_ROW; row++) {
//			for (int col = 0; col < N_COL; col++) {
//				back_buf[row][col] = backup[row][col];
//			}
//		}
//		display();
//	}
//}

void dialog(char message[]) {
	int i, j;
	int messageLength = strlen(message);

	for (int i = DIALOG_DURATION_SEC; i > 0; i--) {
		gotoxy(4, 10);

		for (int j = 0; j < 20; j++) {
			printf("*");
		}
		printf("\n");

		gotoxy(5, 10);
		printf("*       %d %s        *\n", i, message);

		gotoxy(6, 10);
		for (int j = 0; j < 20; j++) {
			printf("*");
		}

		Sleep(1000);
	}


	for (j = 0; j < 3; j++) {
		for (i = 4; i <= 6; i++) {
			gotoxy(i, 10);
			printf("                     \n");
		}
	}
}
