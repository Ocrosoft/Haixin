#pragma once

#include <set>
#include <map>
#include <list>
#include <cmath>
#include <stack>
#include <queue>
#include <ctime>
#include <string>
#include <cstdio>
#include <vector>
#include <cctype>
#include <climits>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "windows.h"

#define  NEITHER 0
#define BOT 1
#define Player 2
#define  MAX 100

struct Point
{
	Point() { x = y = val = 0; }
	Point(int x, int y) { this->x = x; this->y = y; val = 0; }
	int x, y, val;
};

class TooYoung
{
public:
	TooYoung();
	~TooYoung();
	void startGame(bool botFirst = false);//开始游戏
	void output();//输出
	bool drop(int x, int y, int color);//落子
	void setGrade(int grade);//设置难度等级
private:
	void init();//初始化
	Point AI_1();//一级
	Point AI_2();//二级
	Point AI_3();//三级
	bool Win(int x,int y);//胜利
private:
	int m_board[16][16];//棋盘
	int m_player = Player;//执子方
	int m_win = NEITHER;//胜利方
	int m_grade = 1;//难度
};

