#include "jjuggumi.h"
#include "canvas.h"
#include "keyin.h"

#define FIELD_SIZE_X 30 // 필드 가로 크기
#define FIELD_SIZE_Y 15 // 필드 세로 크기
#define NUM_ITEM     5

char field[FIELD_SIZE_Y][FIELD_SIZE_X];
int px[PLAYER_MAX], py[PLAYER_MAX]; // 플레이어 위치
int itemX[ITEM_MAX], itemY[ITEM_MAX]; // 아이템 위치
int userPlayer = 0; // 사용자 플레이어 번호

void nightgame_init() {
    map_init(FIELD_SIZE_Y, FIELD_SIZE_X);

    // 파일에서 아이템 정보 읽어오기
    FILE* fp;
    fopen_s(&fp, "C:\\Users\\asd\\Desktop\\programing--main\\jjuggumi.dat", "r");

    // 아이템 정보를 읽어와서 배치하는 작업 수행
    for (int i = 0; i < NUM_ITEM; ++i) {
        fscanf_s(fp, "%s%d%d%d", item[i].name, (unsigned int)sizeof(item[i].name),
            &item[i].intel_buf, &item[i].str_buf, &item[i].stamina_buf);

        // 아이템 배치 로직
        int x, y;
        do {
            x = randint(1, (FIELD_SIZE_Y - 2));
            y = randint(1, (FIELD_SIZE_X - 2));
        } while (back_buf[x][y] != ' ' || !placable(x, y));
        back_buf[x][y] = 'I'; // 아이템을 나타내는 문자
        itemX[i] = x;
        itemY[i] = y;
    }

    fclose(fp); // 파일 닫기

    // 플레이어 배치 (0번 플레이어는 사용자로 설정)
    for (int i = 0; i < n_player; ++i) {
        PLAYER* p = &player[i];
        int x, y;

        if (p->is_alive == true) {
            do {
                x = randint(1, (FIELD_SIZE_Y - 2));
                y = randint(1, (FIELD_SIZE_X - 2));
            } while (back_buf[x][y] != ' ' || !placable(x, y));
            back_buf[x][y] = '0' + i; // 플레이어 번호 출력 (0~3)

            px[i] = x;
            py[i] = y;
        }

        if (i == 0) {
            userPlayer = 0; // 사용자 플레이어 번호 설정
        }
    }
}


void exchangeItem(int px, int py, int itemX, int itemY) {
    // 플레이어가 아이템 위치로 이동하여 교환하는 로직
    back_buf[px][py] = ' '; // 플레이어가 아이템을 가지게 됨
    back_buf[itemX][itemY] = '0'; // 아이템을 플레이어가 가지게 됨
}

int calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

//플레이어 수, 아이템 수 조정
void playerItemInteraction(int PlayerNum) {
    // 아이템이 있는지 확인하는 플래그 -> PLAYER 구조체에 있음
    int ignoredItem = -1;
    // 아이템 번호 확인
    for (int i = 0; i < n_item; i++) {
        PLAYER* p = &player[PlayerNum];

        if ((px[PlayerNum] == itemX[i] && abs(py[PlayerNum] - itemY[i]) == 1) ||
            (py[PlayerNum] == itemY[i] && abs(px[PlayerNum] - itemX[i]) == 1)) {

            // player 아이템이 있을 때만 아이템 교환 또는 무시
            if (p->hasitem == true) {
                // 플레이어가 아이템을 가지고 있을 때 교환 여부 묻기
                if (PlayerNum == 0) {
                    printf("플레이어 0은 아이템을 교환하시겠습니까? (y/n)\n");
                    char input;
                    scanf_s("%c", &input, 1);
                    if (input == 'y' || input == 'Y') {
                        exchangeItem(px[0], py[0], itemX[i], itemY[i]);
                        printf("플레이어 0이 아이템을 교환합니다!\n");

                        back_buf[itemX[i]][itemY[i]] = ' ';
                        itemX[i] = 0;
                        itemY[i] = 0;
                    }
                    else {
                        printf("플레이어 0이 아이템을 무시합니다.\n");
                        ignoredItem = i; // 무시한 아이템의 인덱스 저장
                    }
                }
                else {
                    int random = rand() % 2;
                    if (random == 0) {
                        exchangeItem(px[PlayerNum], py[PlayerNum], itemX[i], itemY[i]);
                        printf("플레이어 %d가 아이템을 교환합니다!\n", PlayerNum);

                        back_buf[itemX[i]][itemY[i]] = ' ';
                        itemX[i] = 0;
                        itemY[i] = 0;
                    }
                    else {
                        printf("플레이어 %d가 아이템을 무시합니다.\n", PlayerNum);
                        ignoredItem = i; // 무시한 아이템의 인덱스 저장
                    }
                }
            }
            else {
                printf("플레이어 %d가 아이템을 획득했습니다!\n", PlayerNum);
                p->hasitem = true;
            }
        }
        //return p;
    }

    // 가장 가까운 아이템으로 이동
    int closestItemIdx = -1;
    int minDistance = FIELD_SIZE_X + FIELD_SIZE_Y; // 최대 거리로 초기화

    for (int i = 0; i < n_item; ++i) {
        if (i != ignoredItem) { // 무시한 아이템은 제외
            int distance = calculateDistance(px[PlayerNum], py[PlayerNum], itemX[i], itemY[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestItemIdx = i; // 가장 가까운 아이템의 인덱스 저장
            }
        }
    }

    // 가장 가까운 아이템이 있는 경우 해당 아이템으로 이동 (플레이어 0은 움직이지 않음)
    if (PlayerNum != 0 && closestItemIdx != -1) {
        int dx = itemX[closestItemIdx] - px[PlayerNum];
        int dy = itemY[closestItemIdx] - py[PlayerNum];

        // 가장 가까운 아이템 방향으로 이동
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



// 플레이어들을 이동하는 함수
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

            // 가장 가까운 아이템 방향으로 이동
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

    hideCursor(); // 커서 숨기기
    nightgame_init();

    system("cls");
    display();

    // 게임 루프
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

        //// 필드 갱신
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