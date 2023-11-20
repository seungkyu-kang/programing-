#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define FIELD_SIZE_X 20 // �ʵ� ���� ũ��
#define FIELD_SIZE_Y 10 // �ʵ� ���� ũ��
#define NUM_PLAYERS 4 // �÷��̾� �� (����� �÷��̾� + ������ �÷��̾�)
#define NUM_ITEMS 2 // ������ ��

void exchangeItem(char field[FIELD_SIZE_Y][FIELD_SIZE_X], int playerX, int playerY, int itemX, int itemY) {
    // �÷��̾ ������ ��ġ�� �̵��Ͽ� ��ȯ�ϴ� ����
    field[playerX][playerY] = ' '; // �÷��̾ �������� ������ ��
    field[itemX][itemY] = '0'; // �������� �÷��̾ ������ ��
}

void playerItemInteraction(char field[FIELD_SIZE_Y][FIELD_SIZE_X], int playerX[NUM_PLAYERS], int playerY[NUM_PLAYERS], int itemX[NUM_ITEMS], int itemY[NUM_ITEMS], int userPlayer) {
    int itemFound = -1; // �������� �ִ��� Ȯ���ϴ� �÷���

    // ������ Ȯ��
    for (int i = 0; i < NUM_ITEMS; ++i) {
        if ((playerX[userPlayer] == itemX[i] && abs(playerY[userPlayer] - itemY[i]) == 1) ||
            (playerY[userPlayer] == itemY[i] && abs(playerX[userPlayer] - itemX[i]) == 1)) {
            itemFound = i; // ������ �߰�
            break;
        }
    }

    // �������� ���� ���� ������ ��ȯ �Ǵ� ����
    if (itemFound != -1) {
        printf("�÷��̾� %d�� �������� ȹ���߽��ϴ�!\n", userPlayer);

        // �÷��̾ �������� ������ ���� �� ��ȯ ���� ����
        if (userPlayer == 0) {
            printf("�÷��̾� 0�� �������� ��ȯ�Ͻðڽ��ϱ�? (y/n)\n");
            char input;
            scanf_s(" %c", &input);
            if (input == 'y') {
                exchangeItem(field, playerX[0], playerY[0], itemX[itemFound], itemY[itemFound]);
                printf("�÷��̾� 0�� �������� ��ȯ�մϴ�!\n");
            }
            else {
                printf("�÷��̾� 0�� �������� �����մϴ�.\n");
            }
        }
        else {
            int random = rand() % 2;
            if (random == 0) {
                exchangeItem(field, playerX[userPlayer], playerY[userPlayer], itemX[itemFound], itemY[itemFound]);
                printf("�÷��̾� %d�� �������� ��ȯ�մϴ�!\n", userPlayer);
            }
            else {
                printf("�÷��̾� %d�� �������� �����մϴ�.\n", userPlayer);
            }
        }
    }

    // ��� �÷��̾ ������ ��ȯ �Ǵ� ���� �� �̵�
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y;
        int closestItemX, closestItemY;

        // �� �÷��̾ ���� ����� �������� ã��
        for (int j = 0; j < NUM_ITEMS; ++j) {
            int distance = calculateDistance(playerX[i], playerY[i], itemX[j], itemY[j]);
            if (distance < minDistance) {
                minDistance = distance;
                closestItemX = itemX[j];
                closestItemY = itemY[j];
            }
        }

        // �÷��̾� �̵� ����
        // ...
    }
}

int calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// �÷��̾���� �̵��ϴ� �Լ�
void movePlayers(char field[FIELD_SIZE_Y][FIELD_SIZE_X], int playerX[NUM_PLAYERS], int playerY[NUM_PLAYERS], int itemX[NUM_ITEMS], int itemY[NUM_ITEMS], int userPlayer, int count) {
    if (count % 5 == 0) {
        for (int i = 1; i < NUM_PLAYERS; ++i) {
            int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y;
            int closestItemX, closestItemY;
            for (int j = 0; j < NUM_ITEMS; ++j) {
                int distance = calculateDistance(playerX[i], playerY[i], itemX[j], itemY[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestItemX = itemX[j];
                    closestItemY = itemY[j];
                }
            }

            if (rand() % 2 == 0) {
                if (playerX[i] != closestItemX) {
                    if (playerX[i] < closestItemX && field[playerX[i] + 1][playerY[i]] == ' ') {
                        field[playerX[i]][playerY[i]] = ' ';
                        playerX[i]++;
                        field[playerX[i]][playerY[i]] = '0' + i;
                    }
                    else if (playerX[i] > closestItemX && field[playerX[i] - 1][playerY[i]] == ' ') {
                        field[playerX[i]][playerY[i]] = ' ';
                        playerX[i]--;
                        field[playerX[i]][playerY[i]] = '0' + i;
                    }
                }
                else if (playerY[i] != closestItemY) {
                    if (playerY[i] < closestItemY && field[playerX[i]][playerY[i] + 1] == ' ') {
                        field[playerX[i]][playerY[i]] = ' ';
                        playerY[i]++;
                        field[playerX[i]][playerY[i]] = '0' + i;
                    }
                    else if (playerY[i] > closestItemY && field[playerX[i]][playerY[i] - 1] == ' ') {
                        field[playerX[i]][playerY[i]] = ' ';
                        playerY[i]--;
                        field[playerX[i]][playerY[i]] = '0' + i;
                    }
                }
            }
        }

        // �÷��̾� 0�� �̵� (�����Ӱ� �̵�)
        if (userPlayer == 0) {
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 224) {
                    ch = _getch();
                    if (ch == 72 && field[playerX[userPlayer] - 1][playerY[userPlayer]] == ' ') {
                        field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                        playerX[userPlayer]--;
                        field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                    }
                    else if (ch == 80 && field[playerX[userPlayer] + 1][playerY[userPlayer]] == ' ') {
                        field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                        playerX[userPlayer]++;
                        field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                    }
                    else if (ch == 75 && field[playerX[userPlayer]][playerY[userPlayer] - 1] == ' ') {
                        field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                        playerY[userPlayer]--;
                        field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                    }
                    else if (ch == 77 && field[playerX[userPlayer]][playerY[userPlayer] + 1] == ' ') {
                        field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                        playerY[userPlayer]++;
                        field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                    }
                }
            }
        }
    }
}

void setCursorPosition(int x, int y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void nightgame() {
    char field[FIELD_SIZE_Y][FIELD_SIZE_X]; // �ʵ� (����, ���� ������ ����)
    int playerX[NUM_PLAYERS], playerY[NUM_PLAYERS]; // �÷��̾� ��ġ
    int itemX[NUM_ITEMS], itemY[NUM_ITEMS]; // ������ ��ġ
    int userPlayer = 0; // ����� �÷��̾� ��ȣ

    hideCursor(); // Ŀ�� �����

    // �ʵ� �ʱ�ȭ
    for (int i = 0; i < FIELD_SIZE_Y; ++i) {
        for (int j = 0; j < FIELD_SIZE_X; ++j) {
            if (i == 0 || j == 0 || i == FIELD_SIZE_Y - 1 || j == FIELD_SIZE_X - 1) {
                field[i][j] = '*'; // �ʵ� �׵θ��� '*'�� ǥ��
            }
            else {
                field[i][j] = ' '; // �������� �������� �ʱ�ȭ
            }
        }
    }

    // ������ ���� ��ġ
    srand((unsigned int)time(NULL));
    for (int i = 0; i < NUM_ITEMS; ++i) {
        int x, y;
        do {
            x = rand() % (FIELD_SIZE_Y - 2) + 1;
            y = rand() % (FIELD_SIZE_X - 2) + 1;
        } while (field[x][y] != ' ');
        field[x][y] = 'I';
        itemX[i] = x;
        itemY[i] = y;
    }

    // �÷��̾� ��ġ (0�� �÷��̾�� ����ڷ� ����)
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int x, y;
        do {
            x = rand() % (FIELD_SIZE_Y - 2) + 1;
            y = rand() % (FIELD_SIZE_X - 2) + 1;
        } while (field[x][y] != ' ');
        field[x][y] = '0' + i; // �÷��̾� ��ȣ ��� (0~3)
        playerX[i] = x;
        playerY[i] = y;

        if (i == 0) {
            userPlayer = i; // ����� �÷��̾� ��ȣ ����
        }
    }

    // �ʱ� �ʵ� ���
    for (int i = 0; i < FIELD_SIZE_Y; ++i) {
        for (int j = 0; j < FIELD_SIZE_X; ++j) {
            setCursorPosition(j, i);
            printf("%c", field[i][j]);
        }
    }

    // ���� ����
    int count = 0;
    while (1) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) {
                ch = _getch();
                if (ch == 72 && field[playerX[userPlayer] - 1][playerY[userPlayer]] == ' ') {
                    field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                    playerX[userPlayer]--;
                    field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                }
                else if (ch == 80 && field[playerX[userPlayer] + 1][playerY[userPlayer]] == ' ') {
                    field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                    playerX[userPlayer]++;
                    field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                }
                else if (ch == 75 && field[playerX[userPlayer]][playerY[userPlayer] - 1] == ' ') {
                    field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                    playerY[userPlayer]--;
                    field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                }
                else if (ch == 77 && field[playerX[userPlayer]][playerY[userPlayer] + 1] == ' ') {
                    field[playerX[userPlayer]][playerY[userPlayer]] = ' ';
                    playerY[userPlayer]++;
                    field[playerX[userPlayer]][playerY[userPlayer]] = '0' + userPlayer;
                }
                else if (ch == 'q') {
                    break; // ����
                }
            }
        }
        movePlayers(field, playerX, playerY, itemX, itemY, userPlayer, count);
        playerItemInteraction(field, playerX, playerY, itemX, itemY, userPlayer);

        // �ʵ� ����
        for (int i = 1; i < FIELD_SIZE_Y - 1; ++i) {
            for (int j = 1; j < FIELD_SIZE_X - 1; ++j) {
                setCursorPosition(j, i);
                printf("%c", field[i][j]);
            }
        }
        count++;
        Sleep(50);
    }
    
}
