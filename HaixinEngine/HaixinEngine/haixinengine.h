#ifndef HAIXINENGINE_H
#define HAIXINENGINE_H

//#include "haixinengine_global.h"
#include <QString>
#include <QStack>
#define  Piece_Black 0
#define  Piece_White 1
#define  COM 0
#define  PLAYER 1
#define  NEITHER -1

class QPoint;
class _declspec(dllexport) HaixinEngine
{
public:
	HaixinEngine();
	~HaixinEngine();
	/*重新开始*/
	void restart();
	/*设置黑子方*/
	void setBlack(int play);
	/*设置白字方*/
	void setWhite(int play);
	/*为color方落子，返回落子点，从1开始，无法落子返回(-1,-1)*/
	QPoint calculate(int color);
	/*color在point处落子，color方必须为玩家，无法落子返回false，可以落子时返回true*/
	bool drop(QPoint point, int color);
	/*获取上一次出错的错误信息*/
	QString getLastErrorMessage();
	/*返回版本信息*/
	QString getInfo();
	/*返回当前棋子个数*/
	int getPieceCount();
	/*上一步*/
	bool previousStep();
	/*下一步*/
	bool nextStep();
	/*获取当前执子方*/
	int getCurrentPlayer();
	/*游戏是否结束*/
	bool isFinished();
	/*获取黑子执子方*/
	int getBlack();
	/*获取白子执子方*/
	int getWhite();
	/*获取上一条分析信息*/
	QString getLastLog();

private:
	bool win(int x, int y);//胜利判断

private:
	int m_board[20][20];//棋盘
	int black_play;
	int white_play;
	QString m_errorMessage;
	QString version = "0.1.0";
	bool finished;
	int piece_count;
	QStack<QPair<QPoint, int> > pieceSave;
	QStack<QPair<QPoint, int> > pieceSaveTmp;
	int who_play;
	int m_logMessage;

};
#endif // HAIXINENGINE_H
