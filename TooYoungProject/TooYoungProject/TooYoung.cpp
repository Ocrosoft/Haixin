#include "TooYoung.h"

using namespace std;

TooYoung::TooYoung() {}
TooYoung::~TooYoung() {}
void TooYoung::startGame(bool botFirst)
{
	init();
	if (botFirst)m_player = BOT;
	else m_player = Player;
	if (!botFirst)output();

	while (1)
	{
		if (m_win)break;
		switch (m_player)
		{
		case Player:
			while(1)
			{
				printf("请输入落子位置(输入形式：行 列)：");
				int x, y; cin >> x >> y;
				if (drop(x, y, Player))
				{
					if (Win(x, y))m_win = Player;
					output();
					break;
				}
				else printf("无法落子，请重新输入\n");
			}
			m_player = BOT;
			break;
		case BOT:
			output();
			printf("思考中...\n");
			Point p(0, 0);
			switch (m_grade)
			{
			case 1:
				p = AI_1();
				break;
			case 2:
				p = AI_2();
				break;
			}
			drop(p.x, p.y, BOT);
			output();
			printf("AI落子：(%d,%d)\n", p.x, p.y);
			if (Win(p.x, p.y))m_win = BOT;
			m_player = Player;
			break;
		}
	}

	if (m_win == Player)printf("你赢了！\n");
	else printf("你输了...\n");
}
void TooYoung::output()
{
	system("cls");
	for (int i = 0; i <= 15; i++)
		if (i)printf("\t%d", i);
	printf("\n\n\n");
	for (int i =1; i <= 15; i++)
	{
		printf("%d\t", i);
		for (int j = 1; j <= 15; j++)
		{
			if(m_board[i][j]==1)printf("○\t");
			else if(m_board[i][j]==2)printf("●\t");
			else printf("┼\t");
		}
		printf("\n\n\n");
	}
}
bool TooYoung::drop(int x, int y,int color)
{
	if (m_board[x][y])return false;
	m_board[x][y] = color;
	return true;
}
void TooYoung::setGrade(int grade)
{
	m_grade = grade;
}
/*-------------*/
void  TooYoung::init()
{
	memset(m_board, 0, sizeof(m_board));
}
Point TooYoung::AI_1()
{
	srand(time(0));
	while (1)
	{
		int x = rand() % 15 + 1;
		Sleep(1000);
		int y = rand() % 15 + 1;
		if (m_board[x][y] == 0)
			return Point(x, y);
	}
}
Point TooYoung::AI_2()
{
	int valueation[16][16][8][2];
	int dir[8][2] = { 0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0,-1,-1 };
	memset(valueation, 0, sizeof(valueation));
#pragma region .   对每个空位置，求的8方向上的玩家子数量
	for (int i = 1; i <= 15; i++)
	{
		for (int j = 1; j <= 15; j++)
		{
			if (m_board[i][j])continue;//如果有子，不作评估
			for (int k = 0; k < 8; k++)//评估玩家的子
			{
				int cnt = 0;
				int tx = i, ty = j;
				for (int t = 0; t < 5; t++)
				{
					tx += dir[k][0];
					ty += dir[k][1];
					if (tx > 15 || tx < 1 || ty>15 || ty < 1)break;
					if (m_board[tx][ty] == Player)cnt++;
					else break;
				}
				valueation[i][j][k][0] = cnt;
			}
			for (int k = 0; k < 8; k++)//评估AI的子
			{
				int cnt = 0;
				int tx = i, ty = j;
				for (int t = 0; t < 5; t++)
				{
					tx += dir[k][0];
					ty += dir[k][1];
					if (tx > 15 || tx < 1 || ty>15 || ty < 1)break;
					if (m_board[tx][ty] == BOT)cnt++;
					else break;
				}
				valueation[i][j][k][1] = cnt;
			}
		}
	}
#pragma endregion
#pragma region .   对每个空位置进行阴线估值
	int win, val[16][16][2]; memset(val, 0, sizeof(val));
	for (int i = 1; i <= 15; i++)
	{
		for (int j = 1; j <= 15; j++)
		{
			if (m_board[i][j])continue;
			win = 0;//玩家子阴线评估
			for (int k = 0; k < 4; k++)
			{
				if (valueation[i][j][k][0] + valueation[i][j][k + 4][0] >= 4)
					win += 10000;
				else if (valueation[i][j][k][0] + valueation[i][j][k + 4][0] == 3)
					win += 1000;
				else if (valueation[i][j][k][0] + valueation[i][j][k + 4][0] == 2)
					win += 100;
				else if (valueation[i][j][k][0] + valueation[i][j][k + 4][0] == 1)
					win += 10;
			}
			val[i][j][0] = win;
			win = 0;//AI子阴线评估
			for (int k = 0; k < 4; k++)
			{
				if (valueation[i][j][k][1] + valueation[i][j][k + 4][1] >= 4)
					win += 10000;
				else if (valueation[i][j][k][1] + valueation[i][j][k + 4][1] == 3)
					win += 1000;
				else if (valueation[i][j][k][1] + valueation[i][j][k + 4][1] == 2)
					win += 100;
				else if (valueation[i][j][k][1] + valueation[i][j][k + 4][1] == 1)
					win += 10;
			}
			val[i][j][1] = win;
		}
	}
#pragma endregion
	int x, y, val_max = 0;
	for (int i = 1; i <= 15; i++)
		for (int j = 0; j <= 15; j++)
			if (val[i][j][0]>val_max)
				x = i, y = j, val_max = val[i][j][0];
	/*暂时不用
	for (int i = 1; i <= 15; i++)
		for (int j = 0; j <= 15; j++)
			if (val[i][j][0] > val_max)
				x = i, y = j, val_max = val[i][j][0];
	*/
	if (!val_max)return AI_1();//如果没有最大，则选择随机落子
	else return Point(x, y);
}
bool TooYoung::Win(int x,int y)
{
	int count = 0;
	int winflag = 1;
	int cur= m_board[x][y];
	int i, j;
	//垂直
	//上
	for (i = x - 1, j = y; i >= 0 && count++ < 5; i--)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	//下
	count = 0;
	for (i = x + 1, j = y; i <= 15 && count++ < 5; i++)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	if (winflag >= 5)return true;
	else winflag = 1;
	//水平
	//右
	for (i = x, j = y + 1; j <= 15 && count++ < 5; j++)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	//左
	for (i = x, j = y - 1; j >= 0 && count++ < 5; j--)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	if (winflag >= 5)return true;
	else winflag = 1;
	//主对角线
	//右下
	for (i = x + 1, j = y + 1; i <=15 && j < 3 && count++ < 5; i++, j++)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	//左上
	for (i = x - 1, j = y - 1; i >= 0 && j >= 0 && count++ < 5; i--, j--)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	if (winflag >= 5)return true;
	else winflag = 1;
	//斜对角线
	//右上
	for (i = x - 1, j = y + 1; i >= 0 && j <=15 && count++ < 5; i--, j++)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	//左下
	for (i = x + 1, j = y - 1; i <= 15 && y >= 0 && count++ < 5; i++, j--)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	if (winflag >= 5)return true;
	else winflag = 1;
	return false;
}