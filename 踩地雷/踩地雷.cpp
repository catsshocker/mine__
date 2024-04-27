#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <Windows.h>
#include <iomanip>
#include <conio.h>
#include <math.h>

using namespace std;

void gotoxy(int, int);
int move();
void outputMap();
int openMap(int, int, int);

int hard = 5; // 難度(地雷數量)

int x = 2, y = 2; // 游標位置
int map[22][22] = {0};
int view[20][20] = {0}; // 0=>關 1=>開 -1=>插旗

int foundBoom = 0;
int findBoomNum = 0;

int main()
{
	srand((unsigned)time(NULL)); // 重設種子碼

	printf("規則說明 踩地雷:\n在選擇好位置後\n\t\",\"(\"<\")=>開啟方塊\n\t\".\"(\">\")=>標記地雷\n\t\"/\"(\"?\")=>取消標記地雷\n按下ENTER確認即可\n\n輸入難度(地雷數量):");
	cin >> hard;

	// 地雷生成
	for (int i = 0; i < hard; i++)
	{
		while (1)
		{
			int r = (int)(rand()) % 400;
			if (map[r / 20 + 1][r % 20 + 1] == 0)
			{
				map[r / 20 + 1][r % 20 + 1] = -1;
				break;
			}
		}
	}

	// 生成數字
	for (int i = 1; i <= 20; i++)
	{
		for (int j = 1; j <= 20; j++)
		{
			if (map[i][j] == 0)
				map[i][j] = (map[i - 1][j - 1] == -1) + (map[i - 1][j] == -1) + (map[i - 1][j + 1] == -1) + (map[i][j - 1] == -1) + (map[i][j + 1] == -1) + (map[i + 1][j - 1] == -1) + (map[i + 1][j] == -1) + (map[i + 1][j + 1] == -1);
		}
	}
	int result;
	double Tstart, Tend;

	// 以下為遊戲迴圈
	system("psuse");
	Tstart = clock();

	while (1)
	{
		outputMap();
		if (hard == foundBoom)
		{
			system("cls");
			printf("恭喜你找到所有的地雷\n");
			break;
		}
		result = openMap(x / 2 - 1, y, move());
		// 踩到地雷 死亡
		if (result == -1)
		{
			gotoxy(0, 21);
			printf("你被炸死了!!!\n累計找到正確地雷 %2d / %2d", findBoomNum, hard);
			break;
		}
	}
	Tend = clock();
	cout << "\n總共耗時 " << (int)((Tend - Tstart) / CLOCKS_PER_SEC) << " s" << endl;
	system("pause");
}
/*



*/
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos;
	scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

int move()
{
	int a;
	int t = 0; // 狀態
	while (1)
	{
		gotoxy(x, y);
		a = _getch();
		if (a == 75)
			x -= 2;
		else if (a == 77)
			x += 2;
		else if (a == 72)
			y -= 1;
		else if (a == 80)
			y += 1;
		else if (a == 44)
		{
			t = 1;
		}
		else if (a == 46)
		{
			t = -1;
		}
		else if (a == 47)
		{
			t = 0;
		}
		else if (a == 13)
		{
			break;
		}

		if (x > 40)
			x = 40;
		else if (x < 2)
			x = 2;
		if (y > 19)
			y = 19;
		else if (y < 0)
			y = 0;
	}
	return t;
}
/*t=1 open */

void outputMap()
{
	system("cls");
	for (int i = 1; i <= 20; i++)
	{
		for (int j = 1; j <= 20; j++)
		{
			if (view[i - 1][j - 1] == 0)
				cout << setw(2) << " ■";
			else if (view[i - 1][j - 1] == -1)
				cout << setw(2) << " ▲";
			else
				cout << setw(2) << map[i][j];
		}
		cout << endl;
	}
	printf("找到 %2d / %2d", findBoomNum, hard);
}
/*openMap
輸入座標
開啟*/
/*
int openMap(int posy,int posx ,int t) {
	//座標為view的坐標系
	if (t == -1) {
		view[posx][posy] = -1;
		return 1;
	}
	else if(t==1){
		view[posx][posy] = 1;
		if (map[posx + 1][posy + 1] == -1) {
			return 0;//開道炸彈
		}
		else if (map[posx + 1][posy + 1] != 0) {//開起來是數字
			return 1;
		}
		else if (map[posx + 1][posy + 1] == 0) {//開起來是0
			return openMap(posx-1,posy-1,1)*openMap(posx-1,posy,1)*openMap(posx-1,posy+1,1)*openMap(posx,posy-1,1) * openMap(posx, posy+1, 1) * openMap(posx+1, posy-1, 1) * openMap(posx+1, posy, 1) * openMap(posx+1, posy+1, 1);
		}
	}
	else {
		return 0;
	}
}
*/
int openMap(int posy, int posx, int t)
{

	// ?查坐?是否超出地?范?
	if (posx < 0 || posx >= 20 || posy < 0 || posy >= 20)
		return 0;

	// 標記炸彈
	if (t == -1)
	{
		view[posx][posy] = -1;
		findBoomNum++;
		if (map[posx + 1][posy + 1] == -1)
			foundBoom++;
		return 1;
	}

	// 取消標記炸彈
	if (t == 0)
	{
		view[posx][posy] = 0;
		findBoomNum--;
		if (map[posx + 1][posy + 1] == -1)
			foundBoom--;
		return 1;
	}

	// ?查?前?元格是否已?被打?或插旗
	if (view[posx][posy] != 0)
		return 0;

	// ???前?元格?已打?
	view[posx][posy] = 1;

	// 如果?前?元格的值?0，??打?相?的?元格
	if (map[posx + 1][posy + 1] == 0)
	{
		// ??打?相?的8??元格
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				// 避免重复???用?前?元格
				if (i != 0 || j != 0)
					openMap(posy + j, posx + i, 1);
			}
		}
	}
	if (map[posx + 1][posy + 1] == -1)
	{
		return -1;
	}

	// 返回1表示成功打??元格
	return 1;
}
