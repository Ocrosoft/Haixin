#include "HaixinEngine.h"
#include <QPoint>
#include <QPair>
#include <QVector>
#include <QTime>

HaixinEngine::HaixinEngine()
{
	restart();
}

HaixinEngine::~HaixinEngine() {}

bool HaixinEngine::win(int x, int y)
{
	if (pieceSave.size() == 225)return true;
	int count = 0;
	int winflag = 1;
	int cur = m_board[x][y];
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
	for (i = x + 1, j = y + 1; i <= 15 && j <= 15 && count++ < 5; i++, j++)
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
	for (i = x - 1, j = y + 1; i >= 0 && j <= 15 && count++ < 5; i--, j++)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	//左下
	for (i = x + 1, j = y - 1; i <= 15 && j >= 0 && count++ < 5; i++, j--)
	{
		if (m_board[i][j] == cur)winflag++;
		else break;
	}
	count = 0;
	if (winflag >= 5)return true;
	return false;
}

QPoint HaixinEngine::calculate(int color)
{
	if (black_play == NEITHER || white_play == NEITHER)
	{
		m_errorMessage = "can't calculate! set player first!";
		return QPoint(-1, -1);
	}
	if (finished)
	{
		m_errorMessage = "game has finished!";
		return QPoint(-1, -1);
	}
	int valuation[16][16][16][16];
	int dir[8][2] = { 0,-1,1,-1,1,0,1,1,0,1,-1,1,-1,0,-1,-1 };
	memset(valuation, 0, sizeof(valuation));
	/*对每个空位置，求8方向上的玩家子数量*/
	for (int i = 1; i <= 15; i++)
	{
		for (int j = 1; j <= 15; j++)
		{
			if (m_board[i][j] != NEITHER)continue;//如果有子，不作评估
												  /*评估对方的子*/
			for (int k = 0; k < 8; k++)
			{
				int cnt = 0;
				int tx = i, ty = j;
				for (int t = 0; t < 5; t++)
				{
					tx += dir[k][0];
					ty += dir[k][1];
					if (tx > 15 || tx < 1 || ty>15 || ty < 1)break;
					if (m_board[tx][ty] == !color)cnt++;
					else break;
				}
				valuation[i][j][k][0] = cnt;
			}
			/*评估己方的子*/
			for (int k = 0; k < 8; k++)
			{
				int cnt = 0;
				int tx = i, ty = j;
				for (int t = 0; t < 5; t++)
				{
					tx += dir[k][0];
					ty += dir[k][1];
					if (tx > 15 || tx < 1 || ty>15 || ty < 1)break;
					if (m_board[tx][ty] == color)cnt++;
					else break;
				}
				valuation[i][j][k][1] = cnt;
			}
		}
	}
	/*对每个空位置进行阴线估值*/
	int win, val[16][16][2]; memset(val, 0, sizeof(val));
	for (int i = 1; i <= 15; i++)
	{
		for (int j = 1; j <= 15; j++)
		{
			if (m_board[i][j] != NEITHER)continue;
			win = 0;//对方子阴线评估
			for (int k = 0; k < 4; k++)
			{
				if (valuation[i][j][k][0] + valuation[i][j][k + 4][0] >= 4)
					win += 10000;
				else if (valuation[i][j][k][0] + valuation[i][j][k + 4][0] == 3)
					win += 1000;
				else if (valuation[i][j][k][0] + valuation[i][j][k + 4][0] == 2)
					win += 100;
				else if (valuation[i][j][k][0] + valuation[i][j][k + 4][0] == 1)
					win += 10;
			}
			val[i][j][0] = win;
			win = 0;//己方子阴线评估
			for (int k = 0; k < 4; k++)
			{
				if (valuation[i][j][k][1] + valuation[i][j][k + 4][1] >= 4)
					win += 10000;
				else if (valuation[i][j][k][1] + valuation[i][j][k + 4][1] == 3)
					win += 1000;
				else if (valuation[i][j][k][1] + valuation[i][j][k + 4][1] == 2)
					win += 100;
				else if (valuation[i][j][k][1] + valuation[i][j][k + 4][1] == 1)
					win += 10;
			}
			val[i][j][1] = win;
		}
	}
	/*找出最不利点*/
	QVector<QPoint> save;
	int x, y, val_max = 0;
	for (int i = 1; i <= 15; i++)
		for (int j = 1; j <= 15; j++)
		{
			if (val[i][j][0] > val_max)
				x = i, y = j, val_max = val[i][j][0], save.clear(), save.push_back(QPoint(x, y));
			else if (val[i][j][0] == val_max)
				save.push_back(QPoint(i, j));
		}
	/*找出最有利点*/
	int max1 = val_max;
	QVector<QPoint> save2;
	val_max = 0;
	for (int i = 1; i <= 15; i++)
		for (int j = 1; j <= 15; j++)
		{
			if (val[i][j][1] > val_max)
				x = i, y = j, val_max = val[i][j][1], save2.clear(), save2.push_back(QPoint(x, y));
			else if (val[i][j][1] == val_max)
				save2.push_back(QPoint(i, j));
		}

	pieceSaveTmp.clear();
	piece_count++;
	who_play = !who_play;

	if (val_max > max1)//进攻
	{
		if (save2.size() == 0)
		{
			m_board[8][8] = color;
			pieceSave.push(qMakePair(QPoint(8, 8), color));
			finished = HaixinEngine::win(8, 8);
			return QPoint(8, 8);
		}
		else if (save2.size() == 1)
		{
			m_board[save2[0].x()][save2[0].y()] = color;
			pieceSave.push(qMakePair(save2[0], color));
			finished = HaixinEngine::win(save2[0].x(), save2[0].y());
			return save2[0];
		}
		else
		{
			qsrand(QTime::currentTime().msec());
			auto tmp = qrand() % save2.size();
			m_board[save2[tmp].x()][save2[tmp].y()] = color;
			pieceSave.push(qMakePair(save2[tmp], color));
			finished = HaixinEngine::win(save2[tmp].x(), save2[tmp].y());
			return save2[tmp];
		}
	}

	if (save.size() == 0 || max1 == 0)//防守
	{
		m_board[8][8] = color;
		pieceSave.push(qMakePair(QPoint(8, 8), color));
		finished = HaixinEngine::win(8, 8);
		return QPoint(8, 8);
	}
	else if (save.size() == 1)
	{
		m_board[save[0].x()][save[0].y()] = color;
		pieceSave.push(qMakePair(save[0], color));
		finished = HaixinEngine::win(save[0].x(), save[0].y());
		return save[0];
	}
	else
	{
		qsrand(QTime::currentTime().msec());
		auto tmp = qrand() % save.size();
		m_board[save[tmp].x()][save[tmp].y()] = color;
		pieceSave.push(qMakePair(save[tmp], color));
		finished = HaixinEngine::win(save[tmp].x(), save[tmp].y());
		return save[tmp];
	}
}

void HaixinEngine::setBlack(int play)
{
	black_play = play;
}

void HaixinEngine::setWhite(int play)
{
	white_play = play;
}

void HaixinEngine::restart()
{
	memset(m_board, NEITHER, sizeof(m_board));
	black_play = NEITHER;
	white_play = NEITHER;
	finished = false;
	piece_count = 1;
	who_play = Piece_Black;
}

bool HaixinEngine::drop(QPoint point, int color)
{
	if (finished)
	{
		m_errorMessage = "game has finished!";
		return false;
	}
	if (color == Piece_Black&&black_play == COM)
	{
		m_errorMessage = QString("you can't drop!");
		return false;
	}
	if (color == Piece_White&&white_play == COM)
	{
		m_errorMessage = QString("you can't drop!");
		return false;
	}
	if (m_board[point.x()][point.y()] != NEITHER)
	{
		m_errorMessage = QString("can't drop here!");
		return false;
	}
	pieceSaveTmp.clear();
	piece_count++;
	who_play = !who_play;
	m_board[point.x()][point.y()] = color;
	pieceSave.push(qMakePair(point, color));
	finished = HaixinEngine::win(point.x(), point.y());
	return true;
}

QString HaixinEngine::getLastErrorMessage()
{
	return m_errorMessage;
}

QString HaixinEngine::getInfo()
{
	return QString("HAIXIN 2017 ENGINE " + version + " :-)");
}

int HaixinEngine::getPieceCount()
{
	return piece_count;
}

bool HaixinEngine::previousStep()
{
	if (pieceSave.empty()) { m_errorMessage = QString("No previous step!"); return false; }
	pieceSaveTmp.push(pieceSave.top());
	pieceSave.pop();
	m_board[pieceSaveTmp.top().first.x()][pieceSaveTmp.top().first.y()] = NEITHER;
	piece_count--;
	who_play = !who_play;
	finished = false;
	return true;
}

bool HaixinEngine::nextStep()
{
	if (pieceSaveTmp.empty()) { m_errorMessage = QString("No next step!"); return false; }
	pieceSave.push(pieceSaveTmp.top());
	pieceSaveTmp.pop();
	m_board[pieceSave.top().first.x()][pieceSave.top().first.y()] = pieceSave.top().second;
	piece_count++;
	who_play = !who_play;
	//if (piece_count >= 225)finished = true;
	return true;
}

int HaixinEngine::getCurrentPlayer()
{
	return who_play;
}

bool HaixinEngine::isFinished()
{
	return finished;
}

int HaixinEngine::getBlack()
{
	return black_play;
}

int HaixinEngine::getWhite()
{
	return white_play;
}

QString HaixinEngine::getLastLog()
{
	return m_logMessage;
}