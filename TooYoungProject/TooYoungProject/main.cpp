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
			printf("����������(0Ϊ�������,1ΪAI����)��");
			int botFirst; cin >> botFirst;
			auto tooYoung = new TooYoung();
			printf("��ѡ���Ѷ�(1-2,�Ѷ�����)��");
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
			printf("/start\t��ʼ��Ϸ\n");
			printf("/exit\t�˳�(��ֱ�ӹرմ���)\n");
			printf("/?\t�鿴����\n");
		}
		else
		{
			printf("δ֪����.ʹ��/?�鿴��������.\n");
		}
	}
	return 0;
}