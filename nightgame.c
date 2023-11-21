#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define FIELD_SIZE_X 30 // �ʵ� ���� ũ��
#define FIELD_SIZE_Y 15 // �ʵ� ���� ũ��
#define NUM_ITEM     5

char field[FIELD_SIZE_Y][FIELD_SIZE_X];
int px[PLAYER_MAX], py[PLAYER_MAX]; // �÷��̾� ��ġ
int itemX[ITEM_MAX], itemY[ITEM_MAX]; // ������ ��ġ
int userPlayer = 0; // ����� �÷��̾� ��ȣ

void nightgame_init() {
    map_init(FIELD_SIZE_Y, FIELD_SIZE_X);

    // ������ ���� ��ġ
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n_item; ++i) {
        ITEM* tem = &item[i];
        int x, y;

        do {
            x = randint(1, (FIELD_SIZE_Y - 2));
            y = randint(1, (FIELD_SIZE_X - 2));
        } while (back_buf[x][y] != ' ' || !placable(x, y));
        back_buf[x][y] = 'I';
        itemX[i] = x;
        itemY[i] = y;
    }

    // �÷��̾� ��ġ (0�� �÷��̾�� ����ڷ� ����)
    for (int i = 0; i < n_player; ++i) {
        PLAYER* p = &player[i];
        int x, y;

        if (p->is_alive == true) {
            do {
                x = randint(1, (FIELD_SIZE_Y - 2));
                y = randint(1, (FIELD_SIZE_X - 2));
            } while (back_buf[x][y] != ' ' || !placable(x, y));
            back_buf[x][y] = '0' + i; // �÷��̾� ��ȣ ��� (0~3)

            px[i] = x;
            py[i] = y;
        }

        if (i == 0) {
            userPlayer = 0; // ����� �÷��̾� ��ȣ ����
        }
    }
}


void exchangeItem(int px, int py, int itemX, int itemY) {
    // �÷��̾ ������ ��ġ�� �̵��Ͽ� ��ȯ�ϴ� ����
    back_buf[px][py] = ' '; // �÷��̾ �������� ������ ��
    back_buf[itemX][itemY] = '0'; // �������� �÷��̾ ������ ��
}

int calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

//�÷��̾� ��, ������ �� ����
void playerItemInteraction(int px[PLAYER_MAX], int py[PLAYER_MAX], int itemX[ITEM_MAX], int itemY[ITEM_MAX], int PlayerNum) {
    int itemFound = -1; // �������� �ִ��� Ȯ���ϴ� �÷���

    // ������ Ȯ��
    for (int i = 0; i < n_item; ++i) {
        if ((px[PlayerNum] == itemX[i] && abs(py[PlayerNum] - itemY[i]) == 1) ||
            (py[PlayerNum] == itemY[i] && abs(px[PlayerNum] - itemX[i]) == 1)) {
            itemFound = i; // i��° ������ �߰�
            break;
        }
    }

    // player0 �������� ���� ���� ������ ��ȯ �Ǵ� ����
    if (itemFound != -1) {
        printf("�÷��̾� %d�� �������� ȹ���߽��ϴ�!\n", PlayerNum);

        // �÷��̾ �������� ������ ���� �� ��ȯ ���� ����
        if (PlayerNum == 0) {
            printf("�÷��̾� 0�� �������� ��ȯ�Ͻðڽ��ϱ�? (y/n)\n");
            char input;
            scanf_s("%c", &input, 1);
            if (input == 'y' || input == 'Y') {
                exchangeItem(px[0], py[0], itemX[itemFound], itemY[itemFound]);
                printf("�÷��̾� 0�� �������� ��ȯ�մϴ�!\n");
            }
            else {
                printf("�÷��̾� 0�� �������� �����մϴ�.\n");
            }
        }
        else {
            int random = rand() % 2;
            if (random == 0) {
                exchangeItem(px[PlayerNum], py[PlayerNum], itemX[itemFound], itemY[itemFound]);
                printf("�÷��̾� %d�� �������� ��ȯ�մϴ�!\n", PlayerNum);
            }
            else {
                printf("�÷��̾� %d�� �������� �����մϴ�.\n", PlayerNum);
            }
        }
    }
   

    // ��� �÷��̾ ������ ��ȯ �Ǵ� ���� �� �̵�
    for (int i = 0; i < n_player; ++i) {
        int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y;
        int closestItemX, closestItemY;

        // �� �÷��̾ ���� ����� �������� ã��
        for (int j = 0; j < n_item; ++j) {
            int distance = calculateDistance(px[i], py[i], itemX[j], itemY[j]);
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

// �÷��̾���� �̵��ϴ� �Լ�
void movePlayers(int px[PLAYER_MAX], int py[PLAYER_MAX], int itemX[ITEM_MAX], int itemY[ITEM_MAX], int userPlayer, int count) {
    if (count % 5 == 0) {
        for (int i = 1; i < PLAYER_MAX; ++i) {
            int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y;
            int closestItemX, closestItemY;
            for (int j = 0; j < n_item; ++j) {
                int distance = calculateDistance(px[i], py[i], itemX[j], itemY[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestItemX = itemX[j];
                    closestItemY = itemY[j];
                }
            }

            if (rand() % 2 == 0) {
                if (px[i] != closestItemX) {
                    if (px[i] < closestItemX && field[px[i] + 1][py[i]] == ' ') {
                        field[px[i]][py[i]] = ' ';
                        px[i]++;
                        field[px[i]][py[i]] = '0' + i;
                    }
                    else if (px[i] > closestItemX && field[px[i] - 1][py[i]] == ' ') {
                        field[px[i]][py[i]] = ' ';
                        px[i]--;
                        field[px[i]][py[i]] = '0' + i;
                    }
                }
                else if (py[i] != closestItemY) {
                    if (py[i] < closestItemY && field[px[i]][py[i] + 1] == ' ') {
                        field[px[i]][py[i]] = ' ';
                        py[i]++;
                        field[px[i]][py[i]] = '0' + i;
                    }
                    else if (py[i] > closestItemY && field[px[i]][py[i] - 1] == ' ') {
                        field[px[i]][py[i]] = ' ';
                        py[i]--;
                        field[px[i]][py[i]] = '0' + i;
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
                    if (ch == 72 && field[px[userPlayer] - 1][py[userPlayer]] == ' ') {
                        field[px[userPlayer]][py[userPlayer]] = ' ';
                        px[userPlayer]--;
                        field[px[userPlayer]][py[userPlayer]] = '0' + userPlayer;
                    }
                    else if (ch == 80 && field[px[userPlayer] + 1][py[userPlayer]] == ' ') {
                        field[px[userPlayer]][py[userPlayer]] = ' ';
                        px[userPlayer]++;
                        field[px[userPlayer]][py[userPlayer]] = '0' + userPlayer;
                    }
                    else if (ch == 75 && field[px[userPlayer]][py[userPlayer] - 1] == ' ') {
                        field[px[userPlayer]][py[userPlayer]] = ' ';
                        py[userPlayer]--;
                        field[px[userPlayer]][py[userPlayer]] = '0' + userPlayer;
                    }
                    else if (ch == 77 && field[px[userPlayer]][py[userPlayer] + 1] == ' ') {
                        field[px[userPlayer]][py[userPlayer]] = ' ';
                        py[userPlayer]++;
                        field[px[userPlayer]][py[userPlayer]] = '0' + userPlayer;
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
    n_item = n_player - 1;

    hideCursor(); // Ŀ�� �����
    nightgame_init();
    
    system("cls");
    display();

    // ���� ����
    int count = 0;
    while (1) {
        key_t key = get_key();
        if (key == K_QUIT) {
            break;
        }
        else if (key != K_UNDEFINED) {
            PLAYER* p0 = &player[0];
            if (p0->is_alive == false) continue;
            move_manual(key);
        }

       // movePlayers(px, py, itemX, itemY, userPlayer, count);
        //playerItemInteraction(field, playerX, playerY, itemX, itemY, userPlayer);

        // �ʵ� ����
        /*for (int i = 1; i < FIELD_SIZE_Y - 1; ++i) {
            for (int j = 1; j < FIELD_SIZE_X - 1; ++j) {
                setCursorPosition(j, i);
                printf("%c", field[i][j]);
            }
        }*/
        
        count++;
        Sleep(50);

        display();
    }
    
}