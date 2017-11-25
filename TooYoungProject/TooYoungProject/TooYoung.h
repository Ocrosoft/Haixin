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
	void startGame(bool botFirst = false);//��ʼ��Ϸ
	void output();//���
	bool drop(int x, int y, int color);//����
	void setGrade(int grade);//�����Ѷȵȼ�
private:
	void init();//��ʼ��
	Point AI_1();//һ��
	Point AI_2();//����
	Point AI_3();//����
	bool Win(int x,int y);//ʤ��
private:
	int m_board[16][16];//����
	int m_player = Player;//ִ�ӷ�
	int m_win = NEITHER;//ʤ����
	int m_grade = 1;//�Ѷ�
};

