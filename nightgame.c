#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define FIELD_SIZE_X 30 // �ʵ� ���� ũ��
#define FIELD_SIZE_Y 15 // �ʵ� ���� ũ��


char field[FIELD_SIZE_Y][FIELD_SIZE_X];
int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX]; // �÷��̾� ��ġ
int itemX[ITEM_MAX], itemY[ITEM_MAX]; // ������ ��ġ

bool ignoreitem[PLAYER_MAX][ITEM_MAX] = { 0 };


//int userPlayer = 0; // ����� �÷��̾� ��ȣ

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
        back_buf[x][y] = 'I'; // �������� ��Ÿ���� ����
        itemX[i] = x;
        itemY[i] = y;
    }

    // �÷��̾� ��ġ, �ֱ�
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
            period[i] = randint(100, 500);
        }
    }
}


void getItem(PLAYER* p, ITEM* tem) {
    strncpy_s(p->item.name, sizeof(p->item.name), tem->name, sizeof(tem->name));
    p->item.intel_buf = tem->intel_buf;
    p->item.str_buf = tem->str_buf;
    p->item.stamina_buf = tem->stamina_buf;
}

// ���Ϸα� ��� ��������
void removeDialog() {
    if (tick % 1000 == 0) {
        gotoxy(16, 0);
        printf("                                                                       ");
        gotoxy(17, 0);
        printf("                                                                            ");
    }
}

void exchangeItem(PLAYER* p, ITEM* tem, int new_itemX, int new_itemY) {
    for (int j = 0; j < n_item; j++) {
        ITEM* tem = &item[j];

        if (strcmp(tem->name, p->item.name) == 0) {
            //�����ִ� �� ������
            itemX[j] = new_itemX;
            itemY[j] = new_itemY;

            //���� �� ����
            ignoreitem[p->id][j] = true;
            break;
        }
    }
    printf("�÷��̾� %d�� ������(%s)�� ", p->id, p->item.name);

    //���ο� �� �Ա�
    getItem(p, tem);
    
    for (int j = 0; j < n_item; j++) {
        ITEM* tem = &item[j];

        if (strcmp(tem->name, p->item.name) == 0) {
            // ���ο� �� ��ġ �ʱ�ȭ
            itemX[j] = 0;
            itemY[j] = 0;

            break;
        }
    }
    printf("������(%s)�� ��ȯ�մϴ�!\n", p->item.name);
}

int calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}


void playerItemInteraction(int PlayerNum) {
    PLAYER* p = &player[PlayerNum];

    // ������ ��ȣ Ȯ��
    for (int i = 0; i < n_item; i++) {
        ITEM* tem = &item[i];
        
        if ((px[p->id] == itemX[i] && abs(py[p->id] - itemY[i]) == 1) ||
            (py[p->id] == itemY[i] && abs(px[p->id] - itemX[i]) == 1)) {

            if (ignoreitem[PlayerNum][i] == true) continue;

            // player �������� ���� ���� ������ ��ȯ �Ǵ� ����
            if (p->hasitem == true) {
                // �÷��̾ �������� ������ ���� �� ��ȯ ���� ����
                if (p->id == 0) {
                    gotoxy(16, 0);
                    printf("�÷��̾� 0�� �������� ��ȯ�Ͻðڽ��ϱ�? (y/n)\n");
                    char input;
                    scanf_s("%c", &input, 1); 
                    getchar();

                    Sleep(500);
                    gotoxy(16, 0);
                    printf("                                                                       ");
                    gotoxy(17, 0);
                    printf("          ");

                    if (input == 'y' || input == 'Y') {
                        getItem(p, tem);

                        gotoxy(17, 0);
                        exchangeItem(p, tem, itemX[i], itemY[i]);

                        //ignoreitem �ʱ�ȭ
                        for (int k = 0; k < n_item; k++) ignoreitem[p->id][k] = false;

                        tick = 10;
                    }
                    else {
                        gotoxy(16, 0);
                        printf("�÷��̾� 0�� ������(%s)�� �����մϴ�.\n", tem->name);
                        tick = 10;

                        ignoreitem[p->id][i] = true;
                    }
                }
                else {
                    int random = rand() % 2;
                    if (random == 0) {
                        //ignoreitem �ʱ�ȭ
                        for (int k = 0; k < n_item; k++) ignoreitem[p->id][k] = false;

                        gotoxy(17, 0);
                        exchangeItem(p, tem, itemX[i], itemY[i]);
                        tick = 10;

                    }
                    else {
                        gotoxy(16, 0);
                        printf("�÷��̾� %d�� ������(%s)�� �����մϴ�.\n", p->id, tem->name);
                        tick = 10;

                        ignoreitem[p->id][i] = true;

                    }
                }
            }
            else {
                p->hasitem = true;                
                getItem(p, tem);

                gotoxy(16, 0);
                printf("�÷��̾� %d�� ������(%s)�� ȹ���߽��ϴ�!\n", PlayerNum, p->item.name);
                tick = 10;

                back_buf[itemX[i]][itemY[i]] = ' ';
                itemX[i] = 0;
                itemY[i] = 0;
            }
        }
    }
}

// �÷��̾���� �̵��ϴ� �Լ�
// �ʿ� ���� ������ 0,0 ���� ��
void movePlayers(int PlayerNum) {
    int i = PlayerNum;
    int minDistance = 100;
    int closestItemX = 0, closestItemY = 0;

    for (int j = 0; j < n_item; ++j) {
        if (ignoreitem[PlayerNum][j] == true) continue;
        if (itemX[j] == 0 && itemY[j] == 0) continue;

        int distance = calculateDistance(px[PlayerNum], py[PlayerNum], itemX[j], itemY[j]);
        if (distance < minDistance) {
            minDistance = distance;
            closestItemX = itemX[j];
            closestItemY = itemY[j];
        }
    }

    int dx = closestItemX - px[PlayerNum];
    int dy = closestItemY - py[PlayerNum];

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
    n_item = 7;

    hideCursor(); // Ŀ�� �����
    nightgame_init();
    
    system("cls");
    display();

    // ���� ����
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

        
        for (int i = 0; i < n_player; i++) {
            PLAYER* p = &player[i];

            if (tick % period[i] < 10) {

                if (i == 0) continue;
                movePlayers(i);
            }
        }


        display();
        tick += 10;
        Sleep(10);
        
        for (int i = 0; i < n_player; i++) {
            PLAYER* p = &player[i];
            
            playerItemInteraction(i);
        }
        removeDialog();
    }
    
}
