#include <iostream>
#include <thread>
#include <cstdlib>
#include <string>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

const short winH = 10; // 場地高
const short winW = 20; // 場地寬
const short appNum = 1; // 蘋果的數量

int path = 1; // 玩家輸入的方向 上下左右:1 2 3 4
COORD apples[appNum] = {}; // 蘋果們的資料

bool isGameOver = false;

void gotoxy(int x, int y) {
    COORD scrn;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = x;
    scrn.Y = y;
    SetConsoleCursorPosition(hOutput, scrn);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO ConCurInf;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &ConCurInf);
    ConCurInf.bVisible = 0;
    SetConsoleCursorInfo(hStdOut, &ConCurInf);
}

void setColor(short color = 7, short backGroundColor = 0) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color + backGroundColor * 16);
}

void getInput() {
    while (!isGameOver) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) {
                ch = _getch();
                if (ch == 72)
                    path = 1;
                else if (ch == 80)
                    path = 2;
                else if (ch == 75)
                    path = 3;
                else if (ch == 77)
                    path = 4;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10)); // 减少CPU占用
    }
}

class Snake {
private:
    COORD body[winH * winW] = {}; // 蛇蛇身體的資料
    int snakeLen = 5; // 蛇蛇的長度
    int sPath = 1; // 上下左右:1 2 3 4
    int moveCounter = 0; // 移动计数器
    const int moveInterval = 4; // 每几次循环移动一次
public:
    int score = 0; // 分數

    Snake() {
        for (int i = 0; i < snakeLen; i++) {
            body[i] = { (int)winW / 2, (short)(winH - snakeLen + i) };
        }
    }

    // 初始化蘋果 遊戲初始化
    void initGame() {
        path = 1;
        srand(time(NULL));
        for (int i = 0; i < appNum; i++) {
            apples[i] = createApple();
        }
    }

    // 創建新的,不重複的蘋果
    COORD createApple() {
        COORD result;
        bool isCover = false;
        while (1) {
            result = { (short)(rand() % winW), (short)(rand() % winH) };
            for (int i = 0; i < snakeLen; i++) {
                if (body[i].Y == result.Y && body[i].X == result.X) {
                    isCover = true;
                    break;
                }
            }

            if (!isCover) {
                return result;
            }
        }
    }

    // 印出蛇蛇
    void showSnake(int move = 0) {
        setColor(5);
        gotoxy(body[0].X + move, body[0].Y + move);
        cout << '#';
        setColor(3);
        for (int i = 1; i < snakeLen; i++) {
            gotoxy(body[i].X + move, body[i].Y + move);
            cout << '0';
        }
    }

    void showApples(int move = 0) {
        setColor(4,12);
        for (int i = 0; i < appNum; i++) {
            gotoxy(apples[i].X + move, apples[i].Y + move);
            cout << 'Q';
        }
    }

    // 顯示遊戲外框
    void showBlock() {
        setColor(2);
        for (short i = 0; i < winW + 2; i++) {
            gotoxy(i, 0);
            cout << '#';
            gotoxy(i, winH + 1);
            cout << '#';
        }
        for (short i = 0; i < winH; i++) {
            gotoxy(0, i + 1);
            cout << '#';
            gotoxy(winW + 1, i + 1);
            cout << '#';
        }
    }

    // 顯示遊戲畫面
    void showView() {
        system("cls"); // 清除屏幕
        showSnake(1);
        showApples(1);
        showBlock();
        gotoxy(0, winH + 2);
        printf("score:%d", score);
    }

    // 更新蛇蛇資料
    int update() {
        moveCounter++;
        if (moveCounter % moveInterval != 0) {
            return 1; // 跳过这次移动
        }

        bool isDied = false;
        COORD head = body[0];
        if (((path % 2 == 0) ? path - 1 : path + 1) != sPath) {
            sPath = path;
        }

        if (sPath == 1)
            head.Y--;
        else if (sPath == 2)
            head.Y++;
        else if (sPath == 3)
            head.X--;
        else if (sPath == 4)
            head.X++;

        for (int i = snakeLen; i > 0; i--) {
            body[i] = body[i - 1];
            if (body[i].Y == head.Y && body[i].X == head.X && i < snakeLen)
                isDied = true;
        }

        body[0] = head;

        if (head.X >= winW || head.X < 0 || head.Y >= winH || head.Y < 0) {
            isDied = true;
        }

        for (int i = 0; i < appNum; i++) {
            if (apples[i].Y == head.Y && apples[i].X == head.X) {
                snakeLen++;
                score++;
                apples[i] = createApple();
            }
        } // 吃到蘋果

        if (isDied) {
            return -1;
        }
        else {
            return 1;
        }
    }

    int getPath() {
        return sPath;
    }

    // 自动选择路径
    void autoPath() {
        COORD head = body[0];
        COORD apple = apples[0];
        int currentDirection = getPath();

        // 优先尝试朝向苹果的方向移动
        if (currentDirection != 2 && head.Y > apple.Y && isValid(head.X, head.Y - 1)) {
            path = 1; // 向上
        }
        else if (currentDirection != 1 && head.Y < apple.Y && isValid(head.X, head.Y + 1)) {
            path = 2; // 向下
        }
        else if (currentDirection != 4 && head.X > apple.X && isValid(head.X - 1, head.Y)) {
            path = 3; // 向左
        }
        else if (currentDirection != 3 && head.X < apple.X && isValid(head.X + 1, head.Y)) {
            path = 4; // 向右
        }
        else {
            // 如果优先路径不可用，寻找其他可用路径
            if (currentDirection != 2 && isValid(head.X, head.Y - 1)) {
                path = 1; // 向上
            }
            else if (currentDirection != 1 && isValid(head.X, head.Y + 1)) {
                path = 2; // 向下
            }
            else if (currentDirection != 4 && isValid(head.X - 1, head.Y)) {
                path = 3; // 向左
            }
            else if (currentDirection != 3 && isValid(head.X + 1, head.Y)) {
                path = 4; // 向右
            }
        }
    }

    // 检查是否是有效的位置
    bool isValid(int x, int y) {
        if (x < 0 || x >= winW || y < 0 || y >= winH) return false;
        for (int i = 0; i < snakeLen; i++) {
            if (body[i].X == x && body[i].Y == y) return false;
        }
        return true;
    }
};

void game() {
    Snake snake;
    snake.initGame();
    int result;
    while (!isGameOver) {
        hideCursor();
        snake.showView();
        result = snake.update();
        if (result == -1) {
            isGameOver = true;
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(100)); // 保持刷新率不变
    }
    cout << "You're Died!!";
}

void handMove() {
    cout << "Start the game"<<endl;
    system("pause");
    thread gameThread(game);
    thread inputThread(getInput);

    gameThread.join();
    inputThread.join();
}

void autoRun() {
    Snake snake;
    snake.initGame();
    int result;
    while (!isGameOver) {
        hideCursor();
        snake.showView();
        snake.autoPath();
        result = snake.update();
        if (result == -1) {
            isGameOver = true;
            break;
        }
        Sleep(100);
    }
    cout << "You're Died!!";
}

int main() {
    autoRun();
}
