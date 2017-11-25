#include <iostream>
#include <vector>
#include <string>
#include "TooYoung.h"

using namespace std;

int main()
{
	string op;
	while (1)
	{
		cin >> op;
		if (op == "/start")
		{
			printf("请输入先手(0为玩家先手,1为AI先手)：");
			int botFirst; cin >> botFirst;
			auto tooYoung = new TooYoung();
			printf("请选择难度(1-2,难度增加)：");
			int grade; cin >> grade;
			tooYoung->setGrade(grade);
			tooYoung->startGame(botFirst);
		}
		else if (op == "/exit")
		{
			return 0;
		}
		else if (op == "/?")
		{
			printf("/start\t开始游戏\n");
			printf("/exit\t退出(或直接关闭窗口)\n");
			printf("/?\t查看帮助\n");
		}
		else
		{
			printf("未知命令.使用/?查看可用命令.\n");
		}
	}
	return 0;
}