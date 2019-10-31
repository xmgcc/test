// snack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>


void DrawChar(int x, int y, char ch);

// ��Ϸ�����б�
// 1. ̰�������������ƶ�
// 2. ̰���߳�ʳ�ﳤ��
// 3. ̰����ײǽ���Ե��Լ���GG
// 4. ��Ϸ��������ӡ�÷�

// �����ͼ�Ĵ�С
#define MAP_WIDTH 60
#define MAP_HEIGHT 20


typedef struct Postion
{
	// x����
	int x;

	// y����
	int y;
} Position;

// ̰���߽ṹ��
typedef struct Snack
{
	// ̰���ߵĳ���
	int size;

	Position pos[MAP_WIDTH * MAP_HEIGHT];
} Snack;

Snack g_snack;

// ʳ��
Position g_food;

int g_score;

void InitFood()
{
	srand( (unsigned)time( NULL ) );

	g_food.x = rand() % MAP_WIDTH;
	g_food.y = rand() % MAP_HEIGHT;

	DrawChar(g_food.x, g_food.y, '#');
}

// ��Ϸ��ʼ̰���ߵĴ�С
void InitSnack()
{
	g_snack.size = 3;

	g_snack.pos[0].x = MAP_WIDTH / 2;
	g_snack.pos[0].y = MAP_HEIGHT / 2;

	g_snack.pos[1].x = MAP_WIDTH / 2 - 1;
	g_snack.pos[1].y = MAP_HEIGHT / 2;

	g_snack.pos[2].x = MAP_WIDTH / 2 - 2;
	g_snack.pos[2].y = MAP_HEIGHT / 2;
}

// ����̰����
void DrawSnack()
{
	for (int i = 0; i < g_snack.size; i++)
	{
		if (i == 0)
		{
			DrawChar(g_snack.pos[i].x, g_snack.pos[i].y, '*');
		}
		else
		{
			DrawChar(g_snack.pos[i].x, g_snack.pos[i].y, '#');
		}
		
	}
}

// ��x, y�����ַ�ch
// DrawChar(30, 10, '#');
void DrawChar(int x, int y, char ch)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;

	// ���ù��λ��
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

	putchar(ch);
}

void InitMap()
{
	// һ��һ��
	for (int i = 0; i <= MAP_HEIGHT; i++)
	{
		for (int j = 0; j <= MAP_WIDTH; j++)
		{
			
			if (j == MAP_WIDTH)
			{
				// �ұ߿�
				printf("|\n");
			}
			else if (i == MAP_HEIGHT)
			{
				// �ױ�
				printf("-");
			}
			else
			{
				printf(" ");
			}

			
		}
	}
}

void Init()
{
	// ���Ƶ�ͼ
	InitMap();

	// ��ʼ��̰����
	InitSnack();

	InitFood();
}

void UpdateScreen()
{
	DrawSnack();
}

// w ��
// s ��
// a ��
// d ��
void SnackMove(int key)
{
	int delta_x = 0;
	int delta_y = 0;

	
	if (key == 'w' || key == 'W')// w ��
	{
		delta_x = 0;
		delta_y = -1;
	}
	else if (key == 's' || key == 'S')// s ��
	{
		delta_x = 0;
		delta_y = 1;
	}
	else if (key == 'a' || key == 'A')// a ��
	{
		delta_x = -1;
		delta_y = 0;
	}
	else if (key == 'd' || key == 'D')// d ��
	{
		delta_x = 1;
		delta_y = 0;
	}
	else
	{
		// ��Ч����
		return;
	}

	// ɾ��β�ڵ�
	DrawChar(g_snack.pos[g_snack.size - 1].x, g_snack.pos[g_snack.size - 1].y, ' ');

	// ��һ���ڵ㡡����ǰһ���ڵ������
	// g_snack.size - 1 :���һ���ڵ������
	// i>0 ������ͷ�ڵ�
	for (int i = g_snack.size - 1; i > 0; i--)
	{
		g_snack.pos[i].x = g_snack.pos[i-1].x;
		g_snack.pos[i].y = g_snack.pos[i-1].y;
	}
	
	g_snack.pos[0].x += delta_x;
	g_snack.pos[0].y += delta_y;
}

// ͷ�ڵ��ʳ���������ͬ
void EatFood()
{
	if (g_snack.pos[0].x == g_food.x &&
		g_snack.pos[0].y == g_food.y)
	{
		g_snack.size++;
		// �µ�β�ڵ��ʳ�������һ��
		g_snack.pos[g_snack.size - 1].x = g_food.x;
		g_snack.pos[g_snack.size - 1].y = g_food.y;

		InitFood();

		g_score += 10;
	}
}

// ײǽ����-1����������0
int HitWall()
{
	if (g_snack.pos[0].x < 0 ||
		g_snack.pos[0].y < 0 ||
		g_snack.pos[0].x > MAP_WIDTH ||
		g_snack.pos[0].y > MAP_HEIGHT)
	{
		return -1;
	}

	return 0;
}

// ͷ�ڵ�������ڵ��Ƿ�������ͬ
// �Ե��Լ�����-1����������0
int EatSelf()
{
	for (i = 1; i < g_snack.size; i++)
	{
		if (g_snack.pos[0].x == g_snack.pos[i].x &&
			g_snack.pos[0].y == g_snack.pos[i].y)
		{
			return -1;
		}
	}
	return 0;
}

void GameLoop()
{
	int key = 0;

	while (1)
	{
		// �����������

		// ����Ƿ��а�������
		if (_kbhit())
		{
			key = _getch();
		}
		
		// ��q�˳���Ϸ
		if (key == 'q' || key == 'Q')
		{
			return;
		}

		// �����ƶ�̰����
		SnackMove(key);

		// ����ײǽ���¼�
		EatFood();

		if (HitWall() < 0)
		{
			return;
		}


		// ���»���
		UpdateScreen();

		// ��ʱ
		Sleep(100);
	}
}

void Score()
{
	system("cls");
	printf("Good Game!\n");
	printf("�÷� %d\n", g_score);
}

int main(int argc, char* argv[])
{
	// ��ʼ��������ͼ
	Init();

	// ��Ϸ����ѭ��������������Ϸ����ˢ�£�����ײǽ���¼�
	GameLoop();

	// ��ӡ�÷�
	Score();
	return 0;
}

