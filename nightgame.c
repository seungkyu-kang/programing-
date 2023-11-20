#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define FIELD_SIZE_X 20 // 필드 가로 크기
#define FIELD_SIZE_Y 10 // 필드 세로 크기
#define NUM_PLAYERS 4 // 플레이어 수 (사용자 플레이어 + 나머지 플레이어)
#define NUM_ITEMS 2 // 아이템 수

void exchangeItem(char field[FIELD_SIZE_Y][FIELD_SIZE_X], int playerX, int playerY, int itemX, int itemY) {
    // 플레이어가 아이템 위치로 이동하여 교환하는 로직
    field[playerX][playerY] = ' '; // 플레이어가 아이템을 가지게 됨
    field[itemX][itemY] = '0'; // 아이템을 플레이어가 가지게 됨
}

void playerItemInteraction(char field[FIELD_SIZE_Y][FIELD_SIZE_X], int playerX[NUM_PLAYERS], int playerY[NUM_PLAYERS], int itemX[NUM_ITEMS], int itemY[NUM_ITEMS], int userPlayer) {
    int itemFound = -1; // 아이템이 있는지 확인하는 플래그

    // 아이템 확인
    for (int i = 0; i < NUM_ITEMS; ++i) {
        if ((playerX[userPlayer] == itemX[i] && abs(playerY[userPlayer] - itemY[i]) == 1) ||
            (playerY[userPlayer] == itemY[i] && abs(playerX[userPlayer] - itemX[i]) == 1)) {
            itemFound = i; // 아이템 발견
            break;
        }
    }

    // 아이템이 있을 때만 아이템 교환 또는 무시
    if (itemFound != -1) {
        printf("플레이어 %d가 아이템을 획득했습니다!\n", userPlayer);

        // 플레이어가 아이템을 가지고 있을 때 교환 여부 묻기
        if (userPlayer == 0) {
            printf("플레이어 0은 아이템을 교환하시겠습니까? (y/n)\n");
            char input;
            scanf_s(" %c", &input);
            if (input == 'y') {
                exchangeItem(field, playerX[0], playerY[0], itemX[itemFound], itemY[itemFound]);
                printf("플레이어 0이 아이템을 교환합니다!\n");
            }
            else {
                printf("플레이어 0이 아이템을 무시합니다.\n");
            }
        }
        else {
            int random = rand() % 2;
            if (random == 0) {
                exchangeItem(field, playerX[userPlayer], playerY[userPlayer], itemX[itemFound], itemY[itemFound]);
                printf("플레이어 %d가 아이템을 교환합니다!\n", userPlayer);
            }
            else {
                printf("플레이어 %d가 아이템을 무시합니다.\n", userPlayer);
            }
        }
    }

    // 모든 플레이어가 아이템 교환 또는 무시 후 이동
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y;
        int closestItemX, closestItemY;

        // 각 플레이어가 가장 가까운 아이템을 찾음
        for (int j = 0; j < NUM_ITEMS; ++j) {
            int distance = calculateDistance(playerX[i], playerY[i], itemX[j], itemY[j]);
            if (distance < minDistance) {
                minDistance = distance;
                closestItemX = itemX[j];
                closestItemY = itemY[j];
            }
        }

        // 플레이어 이동 로직
        // ...
    }
}

int calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// 플레이어들을 이동하는 함수
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

        // 플레이어 0의 이동 (자유롭게 이동)
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
    char field[FIELD_SIZE_Y][FIELD_SIZE_X]; // 필드 (세로, 가로 순으로 설정)
    int playerX[NUM_PLAYERS], playerY[NUM_PLAYERS]; // 플레이어 위치
    int itemX[NUM_ITEMS], itemY[NUM_ITEMS]; // 아이템 위치
    int userPlayer = 0; // 사용자 플레이어 번호

    hideCursor(); // 커서 숨기기

    // 필드 초기화
    for (int i = 0; i < FIELD_SIZE_Y; ++i) {
        for (int j = 0; j < FIELD_SIZE_X; ++j) {
            if (i == 0 || j == 0 || i == FIELD_SIZE_Y - 1 || j == FIELD_SIZE_X - 1) {
                field[i][j] = '*'; // 필드 테두리는 '*'로 표시
            }
            else {
                field[i][j] = ' '; // 나머지는 공백으로 초기화
            }
        }
    }

    // 아이템 랜덤 배치
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

    // 플레이어 배치 (0번 플레이어는 사용자로 설정)
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        int x, y;
        do {
            x = rand() % (FIELD_SIZE_Y - 2) + 1;
            y = rand() % (FIELD_SIZE_X - 2) + 1;
        } while (field[x][y] != ' ');
        field[x][y] = '0' + i; // 플레이어 번호 출력 (0~3)
        playerX[i] = x;
        playerY[i] = y;

        if (i == 0) {
            userPlayer = i; // 사용자 플레이어 번호 설정
        }
    }

    // 초기 필드 출력
    for (int i = 0; i < FIELD_SIZE_Y; ++i) {
        for (int j = 0; j < FIELD_SIZE_X; ++j) {
            setCursorPosition(j, i);
            printf("%c", field[i][j]);
        }
    }

    // 게임 루프
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
                    break; // 종료
                }
            }
        }
        movePlayers(field, playerX, playerY, itemX, itemY, userPlayer, count);
        playerItemInteraction(field, playerX, playerY, itemX, itemY, userPlayer);

        // 필드 갱신
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
