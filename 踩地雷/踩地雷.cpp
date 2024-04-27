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

int hard = 5; // ����(�a�p�ƶq)

int x = 2, y = 2; // ��Ц�m
int map[22][22] = {0};
int view[20][20] = {0}; // 0=>�� 1=>�} -1=>���X

int foundBoom = 0;
int findBoomNum = 0;

int main()
{
	srand((unsigned)time(NULL)); // ���]�ؤl�X

	printf("�W�h���� ��a�p:\n�b��ܦn��m��\n\t\",\"(\"<\")=>�}�Ҥ��\n\t\".\"(\">\")=>�аO�a�p\n\t\"/\"(\"?\")=>�����аO�a�p\n���UENTER�T�{�Y�i\n\n��J����(�a�p�ƶq):");
	cin >> hard;

	// �a�p�ͦ�
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

	// �ͦ��Ʀr
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

	// �H�U���C���j��
	system("psuse");
	Tstart = clock();

	while (1)
	{
		outputMap();
		if (hard == foundBoom)
		{
			system("cls");
			printf("���ߧA���Ҧ����a�p\n");
			break;
		}
		result = openMap(x / 2 - 1, y, move());
		// ���a�p ���`
		if (result == -1)
		{
			gotoxy(0, 21);
			printf("�A�Q�����F!!!\n�֭p��쥿�T�a�p %2d / %2d", findBoomNum, hard);
			break;
		}
	}
	Tend = clock();
	cout << "\n�`�@�Ӯ� " << (int)((Tend - Tstart) / CLOCKS_PER_SEC) << " s" << endl;
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
	int t = 0; // ���A
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
				cout << setw(2) << " ��";
			else if (view[i - 1][j - 1] == -1)
				cout << setw(2) << " ��";
			else
				cout << setw(2) << map[i][j];
		}
		cout << endl;
	}
	printf("��� %2d / %2d", findBoomNum, hard);
}
/*openMap
��J�y��
�}��*/
/*
int openMap(int posy,int posx ,int t) {
	//�y�Ь�view�����Шt
	if (t == -1) {
		view[posx][posy] = -1;
		return 1;
	}
	else if(t==1){
		view[posx][posy] = 1;
		if (map[posx + 1][posy + 1] == -1) {
			return 0;//�}�D���u
		}
		else if (map[posx + 1][posy + 1] != 0) {//�}�_�ӬO�Ʀr
			return 1;
		}
		else if (map[posx + 1][posy + 1] == 0) {//�}�_�ӬO0
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

	// ?�d��?�O�_�W�X�a?�S?
	if (posx < 0 || posx >= 20 || posy < 0 || posy >= 20)
		return 0;

	// �аO���u
	if (t == -1)
	{
		view[posx][posy] = -1;
		findBoomNum++;
		if (map[posx + 1][posy + 1] == -1)
			foundBoom++;
		return 1;
	}

	// �����аO���u
	if (t == 0)
	{
		view[posx][posy] = 0;
		findBoomNum--;
		if (map[posx + 1][posy + 1] == -1)
			foundBoom--;
		return 1;
	}

	// ?�d?�e?����O�_�w?�Q��?�δ��X
	if (view[posx][posy] != 0)
		return 0;

	// ???�e?����?�w��?
	view[posx][posy] = 1;

	// �p�G?�e?���檺��?0�A??��?��?��?����
	if (map[posx + 1][posy + 1] == 0)
	{
		// ??��?��?��8??����
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				// �קK���`???��?�e?����
				if (i != 0 || j != 0)
					openMap(posy + j, posx + i, 1);
			}
		}
	}
	if (map[posx + 1][posy + 1] == -1)
	{
		return -1;
	}

	// ��^1��ܦ��\��??����
	return 1;
}
