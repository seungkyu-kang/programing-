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

    // ���Ͽ��� ������ ���� �о����
    FILE* fp;
    fopen_s(&fp, "C:\\Users\\asd\\Desktop\\programing--main\\jjuggumi.dat", "r");

    // ������ ������ �о�ͼ� ��ġ�ϴ� �۾� ����
    for (int i = 0; i < NUM_ITEM; ++i) {
        fscanf_s(fp, "%s%d%d%d", item[i].name, (unsigned int)sizeof(item[i].name),
            &item[i].intel_buf, &item[i].str_buf, &item[i].stamina_buf);

        // ������ ��ġ ����
        int x, y;
        do {
            x = randint(1, (FIELD_SIZE_Y - 2));
            y = randint(1, (FIELD_SIZE_X - 2));
        } while (back_buf[x][y] != ' ' || !placable(x, y));
        back_buf[x][y] = 'I'; // �������� ��Ÿ���� ����
        itemX[i] = x;
        itemY[i] = y;
    }

    fclose(fp); // ���� �ݱ�

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
void playerItemInteraction(int PlayerNum) {
    // �������� �ִ��� Ȯ���ϴ� �÷��� -> PLAYER ����ü�� ����
    int ignoredItem = -1;
    // ������ ��ȣ Ȯ��
    for (int i = 0; i < n_item; i++) {
        PLAYER* p = &player[PlayerNum];

        if ((px[PlayerNum] == itemX[i] && abs(py[PlayerNum] - itemY[i]) == 1) ||
            (py[PlayerNum] == itemY[i] && abs(px[PlayerNum] - itemX[i]) == 1)) {

            // player �������� ���� ���� ������ ��ȯ �Ǵ� ����
            if (p->hasitem == true) {
                // �÷��̾ �������� ������ ���� �� ��ȯ ���� ����
                if (PlayerNum == 0) {
                    printf("�÷��̾� 0�� �������� ��ȯ�Ͻðڽ��ϱ�? (y/n)\n");
                    char input;
                    scanf_s("%c", &input, 1);
                    if (input == 'y' || input == 'Y') {
                        exchangeItem(px[0], py[0], itemX[i], itemY[i]);
                        printf("�÷��̾� 0�� �������� ��ȯ�մϴ�!\n");

                        back_buf[itemX[i]][itemY[i]] = ' ';
                        itemX[i] = 0;
                        itemY[i] = 0;
                    }
                    else {
                        printf("�÷��̾� 0�� �������� �����մϴ�.\n");
                        ignoredItem = i; // ������ �������� �ε��� ����
                    }
                }
                else {
                    int random = rand() % 2;
                    if (random == 0) {
                        exchangeItem(px[PlayerNum], py[PlayerNum], itemX[i], itemY[i]);
                        printf("�÷��̾� %d�� �������� ��ȯ�մϴ�!\n", PlayerNum);

                        back_buf[itemX[i]][itemY[i]] = ' ';
                        itemX[i] = 0;
                        itemY[i] = 0;
                    }
                    else {
                        printf("�÷��̾� %d�� �������� �����մϴ�.\n", PlayerNum);
                        ignoredItem = i; // ������ �������� �ε��� ����
                    }
                }
            }
            else {
                printf("�÷��̾� %d�� �������� ȹ���߽��ϴ�!\n", PlayerNum);
                p->hasitem = true;
            }
        }
        //return p;
    }

    // ���� ����� ���������� �̵�
    int closestItemIdx = -1;
    int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y; // �ִ� �Ÿ��� �ʱ�ȭ

    for (int i = 0; i < n_item; ++i) {
        if (i != ignoredItem) { // ������ �������� ����
            int distance = calculateDistance(px[PlayerNum], py[PlayerNum], itemX[i], itemY[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestItemIdx = i; // ���� ����� �������� �ε��� ����
            }
        }
    }

    // ���� ����� �������� �ִ� ��� �ش� ���������� �̵� (�÷��̾� 0�� �������� ����)
    if (PlayerNum != 0 && closestItemIdx != -1) {
        int dx = itemX[closestItemIdx] - px[PlayerNum];
        int dy = itemY[closestItemIdx] - py[PlayerNum];

        // ���� ����� ������ �������� �̵�
        if (abs(dx) > abs(dy)) {
            if (dx > 0 && back_buf[px[PlayerNum] + 1][py[PlayerNum]] == ' ') {
                back_buf[px[PlayerNum]][py[PlayerNum]] = ' ';
                px[PlayerNum]++;
                back_buf[px[PlayerNum]][py[PlayerNum]] = '0' + PlayerNum;
            }
            else if (dx < 0 && back_buf[px[PlayerNum] - 1][py[PlayerNum]] == ' ') {
                back_buf[px[PlayerNum]][py[PlayerNum]] = ' ';
                px[PlayerNum]--;
                back_buf[px[PlayerNum]][py[PlayerNum]] = '0' + PlayerNum;
            }
        }
        else {
            if (dy > 0 && back_buf[px[PlayerNum]][py[PlayerNum] + 1] == ' ') {
                back_buf[px[PlayerNum]][py[PlayerNum]] = ' ';
                py[PlayerNum]++;
                back_buf[px[PlayerNum]][py[PlayerNum]] = '0' + PlayerNum;
            }
            else if (dy < 0 && back_buf[px[PlayerNum]][py[PlayerNum] - 1] == ' ') {
                back_buf[px[PlayerNum]][py[PlayerNum]] = ' ';
                py[PlayerNum]--;
                back_buf[px[PlayerNum]][py[PlayerNum]] = '0' + PlayerNum;
            }
        }
    }
}



// �÷��̾���� �̵��ϴ� �Լ�
void movePlayers(int userPlayer, int count) {
    if (count % 5 == 0) {
        for (int i = 1; i < n_player; i++) {
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

            int dx = closestItemX - px[i];
            int dy = closestItemY - py[i];

            // ���� ����� ������ �������� �̵�
            if (abs(dx) > abs(dy)) {
                if (dx > 0 && back_buf[px[i] + 1][py[i]] == ' ') {
                    back_buf[px[i]][py[i]] = ' ';
                    px[i]++;
                    back_buf[px[i]][py[i]] = '0' + i;
                }
                else if (dx < 0 && back_buf[px[i] - 1][py[i]] == ' ') {
                    back_buf[px[i]][py[i]] = ' ';
                    px[i]--;
                    back_buf[px[i]][py[i]] = '0' + i;
                }
            }
            else {
                if (dy > 0 && back_buf[px[i]][py[i] + 1] == ' ') {
                    back_buf[px[i]][py[i]] = ' ';
                    py[i]++;
                    back_buf[px[i]][py[i]] = '0' + i;
                }
                else if (dy < 0 && back_buf[px[i]][py[i] - 1] == ' ') {
                    back_buf[px[i]][py[i]] = ' ';
                    py[i]--;
                    back_buf[px[i]][py[i]] = '0' + i;
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

        movePlayers(userPlayer, count);

        for (int i = 0; i < n_player; i++) {
            PLAYER* p = &player[i];

            playerItemInteraction(i);
        }

        //// �ʵ� ����
        //for (int i = 1; i < FIELD_SIZE_Y - 1; ++i) {
        //    for (int j = 1; j < FIELD_SIZE_X - 1; ++j) {
        //        setCursorPosition(j, i);
        //        printf("%c", field[i][j]);
        //    }
        //}

        count++;
        tick += 100;
        Sleep(500);

        display();



    }

}