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
	/*���¿�ʼ*/
	void restart();
	/*���ú��ӷ�*/
	void setBlack(int play);
	/*���ð��ַ�*/
	void setWhite(int play);
	/*Ϊcolor�����ӣ��������ӵ㣬��1��ʼ���޷����ӷ���(-1,-1)*/
	QPoint calculate(int color);
	/*color��point�����ӣ�color������Ϊ��ң��޷����ӷ���false����������ʱ����true*/
	bool drop(QPoint point, int color);
	/*��ȡ��һ�γ���Ĵ�����Ϣ*/
	QString getLastErrorMessage();
	/*���ذ汾��Ϣ*/
	QString getInfo();
	/*���ص�ǰ���Ӹ���*/
	int getPieceCount();
	/*��һ��*/
	bool previousStep();
	/*��һ��*/
	bool nextStep();
	/*��ȡ��ǰִ�ӷ�*/
	int getCurrentPlayer();
	/*��Ϸ�Ƿ����*/
	bool isFinished();
	/*��ȡ����ִ�ӷ�*/
	int getBlack();
	/*��ȡ����ִ�ӷ�*/
	int getWhite();
	/*��ȡ��һ��������Ϣ*/
	QString getLastLog();

private:
	bool win(int x, int y);//ʤ���ж�

private:
	int m_board[20][20];//����
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
