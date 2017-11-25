#include "haixin.h"
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QPainter>
#include <QMessageBox>
#include <QTextEdit>
#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QFile>
#include <QApplication>
#include <QFileDialog>
#include <QTime>
#include "HaixinEngine.h"

Haixin::Haixin(QWidget  *parent)
	: QMainWindow(parent)
{
	this->setWindowFlags(windowFlags() &~Qt::WindowMaximizeButtonHint);
	/*窗口大小*/
	this->resize(990, 545);
	this->setWindowIcon(QIcon("ico.ico"));
	this->setFixedSize(this->width(), this->height());
}

Haixin::~Haixin() {}

void Haixin::init()
{
	engine = new HaixinEngine();
	/*菜单-游戏*/
	auto menu_game = menuBar()->addMenu(tr("Game"));

	auto qaction = new QAction(tr("New"));
	menu_game->addAction(qaction);
	connect(qaction, SIGNAL(triggered()), this, SLOT(newFile()));

	qaction = new QAction(tr("Load"));
	menu_game->addAction(qaction);
	connect(qaction, SIGNAL(triggered()), this, SLOT(loadFile()));

	qaction = new QAction(tr("Save"));
	menu_game->addAction(qaction);
	connect(qaction, SIGNAL(triggered()), this, SLOT(saveFile()));

	qaction = new QAction(tr("Quit"));
	connect(qaction, SIGNAL(triggered()), this, SLOT(close()));
	menu_game->addAction(qaction);
	/*菜单-设置*/
	menu_setting = menuBar()->addMenu(tr("Players"));

	qaction = new QAction(tr("Computer plays black"));
	qaction->setCheckable(true);
	if (settings["black"] == "COM")qaction->setChecked(true), engine->setBlack(COM);
	else engine->setBlack(PLAYER);
	connect(qaction, SIGNAL(triggered()), this, SLOT(computerChange()));
	menu_setting->addAction(qaction);

	qaction = new QAction(tr("Computer plays white"));
	qaction->setCheckable(true);
	if (settings["white"] == "COM")qaction->setChecked(true), engine->setWhite(COM);
	else engine->setWhite(PLAYER);
	connect(qaction, SIGNAL(triggered()), this, SLOT(computerChange()));
	menu_setting->addAction(qaction);

	/*菜单-显示*/
	menu_display = menuBar()->addMenu((tr("View")));

	qaction = new QAction(tr("Numeration"));
	qaction->setCheckable(true);
	if (settings["number"] == "true")qaction->setChecked(true);
	connect(qaction, SIGNAL(triggered()), this, SLOT(setNumberVisible()));
	menu_display->addAction(qaction);

	qaction = new QAction(tr("Log"));
	qaction->setCheckable(true);
	if (settings["log"] == "true")
		qaction->setChecked(true);
	else this->resize(590, 545);
	connect(qaction, SIGNAL(triggered()), this, SLOT(setLogVisible()));
	menu_display->addAction(qaction);

	qaction = new QAction(tr("Analysis"));
	qaction->setCheckable(true);
	//setAnalysesVisible();
	if (settings["analysis"] == "true")qaction->setChecked(true);
	connect(qaction, SIGNAL(triggered()), this, SLOT(setAnalysesVisible()));
	menu_display->addAction(qaction);

	auto menu_language = new QMenu(tr("Language"));
	auto group = new QActionGroup(this);
	qaction = new QAction(tr("English"));
	qaction->setCheckable(true);
	if (settings["lang"] == "English")qaction->setChecked(true);
	connect(qaction, SIGNAL(triggered()), this, SLOT(toEnglish()));
	menu_language->addAction(group->addAction(qaction));
	qaction = new QAction(QString("简体中文"));
	qaction->setCheckable(true);
	if (settings["lang"] == "Chinese")qaction->setChecked(true);
	connect(qaction, SIGNAL(triggered()), this, SLOT(toChinese()));
	menu_language->addAction(group->addAction(qaction));
	menu_display->addMenu(menu_language);
	/*菜单-帮助*/
	menu_help = menuBar()->addMenu((tr("Help")));

	qaction = new QAction(tr("About"));
	connect(qaction, SIGNAL(triggered()), this, SLOT(HaixinAbout()));
	menu_help->addAction(qaction);
	/*工具栏*/
	toolBar_operation = new QToolBar("Operation");
	toolBar_operation->setMovable(false);
	toolBar_operation->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	toolBar_operation->setIconSize(QSize(16, 16));

	auto font = QFont(toolBar_operation->font());
	font.setFamily("微软雅黑");
	toolBar_operation->setFont(font);

	qaction = new QAction(tr("Undo All"));
	qaction->setIcon(QIcon("Resources/First_Step.png"));
	connect(qaction, SIGNAL(triggered()), this, SLOT(first_Step()));
	toolBar_operation->addAction(qaction);

	qaction = new QAction(tr("Undo"));
	qaction->setIcon(QIcon("Resources/Previous_Step.png"));
	connect(qaction, SIGNAL(triggered()), this, SLOT(previous_Step()));
	toolBar_operation->addAction(qaction);

	qaction = new QAction(tr("Redo"));
	qaction->setIcon(QIcon("Resources/Next_Step.png"));
	connect(qaction, SIGNAL(triggered()), this, SLOT(next_Step()));
	toolBar_operation->addAction(qaction);

	qaction = new QAction(tr("Redo All"));
	qaction->setIcon(QIcon("Resources/Final_Step.png"));
	connect(qaction, SIGNAL(triggered()), this, SLOT(final_Step()));
	toolBar_operation->addAction(qaction);

	toolBar_operation->setFixedWidth(70);
	addToolBar(Qt::LeftToolBarArea, toolBar_operation);

	logWidget = new QTextEdit();
	//edit->setReadOnly(true);
	logWidget->setFrameStyle(logWidget->NoFrame);
	logWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	logWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	logWidget->setWordWrapMode(QTextOption::NoWrap);
	logWidget->setFont(font);
	logWidget->setText(tr("Haixin Board 1.0"));
	logWidget->append(engine->getInfo());
	logWidget->append("");
	logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
	logWidget->setGeometry(60 + 32 * 16 + 15, 30, 400, 450);
	logWidget->setParent(this);

	cmdWidget = new QTextEdit();
	cmdWidget->setFrameStyle(cmdWidget->NoFrame);
	cmdWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	cmdWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	cmdWidget->setWordWrapMode(QTextOption::NoWrap);
	cmdWidget->setFont(font);
	cmdWidget->setText(tr("To get help, type help and press Enter here"));
	cmdWidget->setGeometry(60 + 32 * 16 + 15, 30 + 32 * 14, 400, 60);
	cmdWidget->setParent(this);
	cmdWidget->installEventFilter(this);

	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			boardStatu[i][j] = nullptr;
	pieceSave.clear();
	pieceSaveTmp.clear();

	drawGUI();
}

void Haixin::HaixinAbout()
{
	QMessageBox::about(this, tr("About"), tr("<p align='center'><font size='4'><b>Haixin Board</b></font><br/>Version 1.0<br/>(C)2016-2017 Ocrosoft<br/><br/>www.ocrosoft.com</p>"));
}

void Haixin::mousePressEvent(QMouseEvent *event)
{
	int x = event->x(), y = event->y();
	if (x < 75 || y < 30 || x>550 || y>510) { return; }

	if (engine->isFinished())return;
	/*C-VS-C*/
	if (engine->getBlack() == COM&&engine->getWhite() == COM)
	{
		if (cvcing)return;
		if (engine->isFinished() && pieceSave.empty())newFile();//如果已经结束，并且点击了首步，再次点击属于重新开始
		cvcing = 1;
		while (!engine->isFinished() && cvcing)
		{
			auto p = engine->calculate(engine->getCurrentPlayer());
			if (p != invalidPoint)
			{
				QTime t; t.start();
				while (t.elapsed() < 200)QCoreApplication::processEvents();
				drop(p.y() - 1, p.x() - 1, !engine->getCurrentPlayer());
				if (cvcing == 0) { newFile(); return; }
			}
			else return;
		}
		cvcing = 0;
		if (!engine->isFinished())return;//说明中断
		if (pieceSave.size() == 225)
		{
			logWidget->append(tr("Draw!")); 
			logWidget->append(""); 
			logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		}
		else if (engine->getCurrentPlayer() == Piece_Black)
		{
			logWidget->append(tr("White Win!"));
			logWidget->append("");
			logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		}
		else
		{
			logWidget->append(tr("Black Win!"));
			logWidget->append("");
			logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		}
		return;
	}
	/*该电脑落子了，适用于返回的情况，点击屏幕让电脑落子*/
	if (engine->getCurrentPlayer() == Piece_Black&&engine->getBlack() == COM ||
		engine->getCurrentPlayer() == Piece_White&&engine->getWhite() == COM)
	{
		auto p = engine->calculate(engine->getCurrentPlayer());
		if (p != invalidPoint)drop(p.y() - 1, p.x() - 1, !engine->getCurrentPlayer());
		if (engine->isFinished())
		{
			if (pieceSave.size() == 225)
			{
				logWidget->append(tr("Draw!"));
				logWidget->append("");
				logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
			}
			else if (engine->getCurrentPlayer() == Piece_Black)
			{
				logWidget->append(tr("White Win!"));
				logWidget->append("");
				logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
			}
			else
			{
				logWidget->append(tr("Black Win!"));
				logWidget->append("");
				logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
			}
		}
		return;
	}
	/*P-VS-C & P-VS-P，玩家落子，电脑自动落子而不用点击屏幕*/
	int xp = (x - 75) / 32, yp = (y - 30) / 32;

	int currentPlayer = engine->getCurrentPlayer();
	if (engine->drop(QPoint(yp + 1, xp + 1), currentPlayer))//成功
	{
		drop(xp, yp, currentPlayer);
		if (engine->isFinished())
		{
			if (pieceSave.size() == 225)
			{
				logWidget->append(tr("Draw!"));
				logWidget->append("");
				logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
			}
			else if (engine->getCurrentPlayer() == Piece_Black)
			{
				logWidget->append(tr("White Win!"));
				logWidget->append("");
				logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
			}
			else
			{
				logWidget->append(tr("Black Win!"));
				logWidget->append("");
				logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
			}
			return;
		}
		if ((currentPlayer == Piece_Black&&engine->getWhite() == COM)
			|| currentPlayer == Piece_White&&engine->getBlack() == COM)//P-VS-C
		{
			auto p = engine->calculate(engine->getCurrentPlayer());
			//QMessageBox::information(this, "", engine->getLastErrorMessage());
			if (p != invalidPoint)drop(p.y() - 1, p.x() - 1, !currentPlayer);
			//else QMessageBox::information(this, "", engine->getLastErrorMessage());
		}
		//P-VS-P 不作处理
	}
	//else QMessageBox::information(this, "", engine->getLastErrorMessage());
	if (engine->isFinished())
	{
		if (pieceSave.size() == 225)
		{
			logWidget->append(tr("Draw!"));
			logWidget->append("");
			logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		}
		else if (engine->getCurrentPlayer() == Piece_Black)
		{
			logWidget->append(tr("White Win!"));
			logWidget->append("");
			logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		}
		else
		{
			logWidget->append(tr("Black Win!"));
			logWidget->append("");
			logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		}
	}
}

void Haixin::drawGUI()
{
	board = new QGraphicsScene();

	auto pen = QPen();
	pen.setColor(QColor(44, 154, 44));//绿
	pen.setCapStyle(Qt::SquareCap);
	pen.setJoinStyle(Qt::RoundJoin);
	board->addRect(60, 30, 480, 480, pen, QBrush(QColor(44, 154, 44)));
	pen.setColor(Qt::black);
	pen.setWidth(2);
	board->addRect(75, 45, 450, 450, pen);
	pen.setWidth(1);
	for (int i = 1; i <= 13; i++)
	{
		board->addLine(75, 45 + i * 32, 525, 45 + i * 32, pen);
		board->addLine(75 + i * 32, 45, 75 + i * 32, 495, pen);
	}
	pen.setWidth(2);
	pen.setCapStyle(Qt::RoundCap);
	board->addEllipse(75 + 32 * 3 - 1, 45 + 32 * 3 - 1, 3, 3, pen, QBrush(Qt::black));
	board->addEllipse(75 + 32 * 3 - 1, 45 + 32 * 11 - 1, 3, 3, pen, QBrush(Qt::black));
	board->addEllipse(75 + 32 * 7 - 1, 45 + 32 * 7 - 1, 3, 3, pen, QBrush(Qt::black));
	board->addEllipse(75 + 32 * 11 - 1, 45 + 32 * 3 - 1, 3, 3, pen, QBrush(Qt::black));
	board->addEllipse(75 + 32 * 11 - 1, 45 + 32 * 11 - 1, 3, 3, pen, QBrush(Qt::black));

	boardView = new QGraphicsView(board, this);
	boardView->setGeometry(75, 30, 485, 485);
}

void Haixin::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	auto pen = QPen();
	pen.setWidth(1);
	painter.setPen(pen);
	painter.setBrush(QBrush());
	painter.setFont(QFont("微软雅黑"));
	for (int i = 0; i <= 14; i++)
	{
		painter.drawText(75 + 15 + 32 * i, 45 + 32 * 15 + 8, QString('A' + i));
		if (15 - i >= 10)
			painter.drawText(75 + 32 * 15 + 10, 45 + 32 * i + 5, QString().setNum(15 - i));
		else painter.drawText(75 + 32 * 15 + 14, 45 + 32 * i + 5, QString().setNum(15 - i));
	}
}

void Haixin::drop(int x, int y, int type)
{
	int offsetx = 0, offsety = 0;
	QString PieceName = type == Piece_Black ? "Black" : "White";
	if (x > 0 && x < 14 && y>0 && y < 14)offsetx = 0, offsety = type == Piece_Black ? 0 : 1, PieceName += "";
	else if (x == 0 && y == 0)offsetx = 0, offsety = type == Piece_Black ? 0 : 1, PieceName += "LT";
	else if (x == 0 && y == 14)offsetx = 0, offsety = type == Piece_Black ? 1 : 2, PieceName += "LB";
	else if (x == 0)offsetx = 0, offsety = type == Piece_Black ? 0 : 1, PieceName += "L";
	else if (y == 0 && x == 14)offsetx = 1, offsety = type == Piece_Black ? 0 : 1, PieceName += "TR";
	else if (y == 0)offsetx = 0, offsety = type == Piece_Black ? 0 : 1, PieceName += "T";
	else if (x == 14 && y == 14)offsetx = 1, offsety = type == Piece_Black ? 1 : 2, PieceName += "RB";
	else if (x == 14)offsetx = 1, offsety = type == Piece_Black ? 0 : 1, PieceName += "R";
	else if (y == 14)offsetx = 0, offsety = 2, PieceName += "B";
	auto piece = new QGraphicsPixmapItem(
		QPixmap("Resources/" + PieceName + ".png"));

	piece->setX(60 + x * 32 + offsetx);
	piece->setY(30 + y * 32 + offsety);
	board->addItem(piece);
	auto font = QFont("微软雅黑");
	font.setBold(true);
	if (engine->getPieceCount() - 1 < 100)font.setPixelSize(14);//<100
	else font.setPixelSize(12);//>=100
	auto index = new QGraphicsTextItem(QString::number(engine->getPieceCount() - 1));
	index->setFont(font);
	index->setDefaultTextColor(Qt::red);
	if (engine->getPieceCount() - 1 < 10)index->setX(60 + x * 32 + 7), index->setY(30 + y * 32 + 2);
	else if (engine->getPieceCount() - 1 < 100)index->setX(60 + x * 32 + 2), index->setY(30 + y * 32 + 2);
	else index->setX(60 + x * 32 + 0), index->setY(30 + y * 32 + 2);
	//engine->getPieceCount()++;
	if (settings["number"]=="false")index->setVisible(false);
	board->addItem(index);
	if (!pieceSave.empty())pieceSave.top().first.second->setDefaultTextColor(type == Piece_Black ? Qt::black : Qt::white);

	boardStatu[y][x] = piece;
	pieceSave.push(qMakePair(qMakePair(piece, index), QPoint(x, y)));

	pieceSaveTmp.clear();//如果落子了，清空后步记录
}

void Haixin::previous_Step()
{
	if (cvcing)
	{
		//cvcing = 0;
		logWidget->append(tr("Please wait the game finished or use Game-New."));
		logWidget->append("");
		logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		return;
	}

	if (pieceSave.empty()) { return; }
	pieceSaveTmp.push(pieceSave.top());
	pieceSave.pop();
	if (!pieceSave.empty())pieceSave.top().first.second->setDefaultTextColor(Qt::red);
	board->removeItem(pieceSaveTmp.top().first.first);
	board->removeItem(pieceSaveTmp.top().first.second);

	engine->previousStep();
}

void Haixin::next_Step()
{
	if (pieceSaveTmp.empty()) { return; }
	if (!pieceSave.empty())pieceSave.top().first.second->setDefaultTextColor(pieceSave.top().first.second->toPlainText().toInt() % 2 ? Qt::white : Qt::black);
	pieceSave.push(pieceSaveTmp.top());
	pieceSaveTmp.pop();
	board->addItem(pieceSave.top().first.first);
	board->addItem(pieceSave.top().first.second);

	engine->nextStep();
}

void Haixin::first_Step()
{
	if (cvcing)
	{
		logWidget->append(tr("Please wait the game finished or use Game-New."));
		logWidget->append("");
		logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		return;
	}
	while (!pieceSave.empty())
	{
		previous_Step();
		//pieceSaveTmp.push(pieceSave.top());
		//pieceSave.pop();
		//board->removeItem(pieceSaveTmp.top().first.first);
		//board->removeItem(pieceSaveTmp.top().first.second);

		engine->previousStep();
	}
}

void Haixin::final_Step()
{
	while (!pieceSaveTmp.empty())
	{
		//pieceSave.push(pieceSaveTmp.top());
		//pieceSaveTmp.pop();
		//board->addItem(pieceSave.top().first.first);
		//board->addItem(pieceSave.top().first.second);
		next_Step();

		engine->nextStep();
	}
}

void Haixin::toChinese()
{
	settings["lang"] = "Chinese";

	QFile file("config.ini");
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Can not save config. Change language failed."));
		return;
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_5_7);
	out << quint32(0x66601000) << settings;

	a->exit(233);
}

void Haixin::toEnglish()
{
	settings["lang"] = "English";

	QFile file("config.ini");
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Can not save config. Change language failed."));
		return;
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_5_7);
	out << quint32(0x66601000) << settings;

	a->exit(233);
}

void Haixin::computerChange()
{
	if (menu_setting->actions().at(0)->isChecked())
	{
		engine->setBlack(COM);
		settings["black"] = "COM";
	}
	else
	{
		engine->setBlack(PLAYER);
		settings["black"] = "PLAYER";
	}

	if (menu_setting->actions().at(1)->isChecked())
	{
		engine->setWhite(COM);
		settings["white"] = "COM";
	}
	else
	{
		engine->setWhite(PLAYER);
		settings["white"] = "PLAYER";
	}
}

void Haixin::commandInput()
{
	auto text = cmdWidget->toPlainText();
	if (text == "help")
	{
		logWidget->append(tr("No commans available now!"));
		logWidget->append("");
		logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
		cmdWidget->setText("");
	}
	else cmdWidget->append("");
}

bool Haixin::eventFilter(QObject *obj, QEvent *e)
{
	Q_ASSERT(obj == cmdWidget);
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *event = static_cast<QKeyEvent*>(e);
		if (event->key() == Qt::Key_Return)
		{
			commandInput();
			return true;
		}
	}
	return false;
}

void Haixin::setNumberVisible()
{
	if (menu_display->actions().at(0)->isChecked())
	{
		auto tmp = pieceSave;
		while (!tmp.empty())
		{
			auto t = tmp.top().first.second;
			t->setVisible(true);
			tmp.pop();
		}
		tmp = pieceSaveTmp;
		while (!tmp.empty())
		{
			auto t = tmp.top().first.second;
			t->setVisible(true);
			tmp.pop();
		}
		settings["number"] = "true";
	}
	else
	{
		auto tmp = pieceSave;
		while (!tmp.empty())
		{
			auto t = tmp.top().first.second;
			t->setVisible(false);
			tmp.pop();
		}
		tmp = pieceSaveTmp;
		while (!tmp.empty())
		{
			auto t = tmp.top().first.second;
			t->setVisible(false);
			tmp.pop();
		}
		settings["number"] = "false";
	}
}

void Haixin::setLogVisible()
{
	if (menu_display->actions().at(1)->isChecked())
	{
		this->resize(990, 545);
		settings["log"] = "true";
	}
	else
	{
		this->resize(590, 545);
		settings["log"] = "false";
	}
}

void Haixin::setAnalysesVisible()
{
	if (menu_display->actions().at(2)->isChecked())
	{
		settings["analysis"] = "true";
	}
	else 
	{
		settings["analysis"] = "false";
	}
}

void Haixin::setSettings(QMap<QString, QString> settings)
{
	this->settings = settings;
}

void Haixin::closeEvent(QCloseEvent *event)
{
	QFile file("config.ini");
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Can not save config."));
		event->accept();
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_5_7);
	out << quint32(0x66601000) << settings;
	event->accept();
}

void Haixin::receiveAPP(QApplication *a)
{
	this->a = a;
}

void Haixin::newFile()
{
	bool flag = 0;
	if (cvcing)cvcing = 0, flag = 1;

	//pieceSave.clear();
	first_Step();//不能pieceSave.clear(),无法清除board中item
	pieceSaveTmp.clear();

	engine->restart();//必须在下面，不然first_Step()会影响engine(engine没有安全检查机制)

	if (settings["black"] == "COM")engine->setBlack(COM);
	else engine->setBlack(PLAYER);
	if (settings["white"] == "COM")engine->setWhite(COM);
	else engine->setWhite(PLAYER);

	if (!pieceSave.empty())newFile();
}

void Haixin::saveFile()
{
	QString path = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("Haixin Save File(*.sav)"));
	if (path.length() != 0)
	{
		QFile file(path);
		if (!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::critical(this, tr("Error"), tr("Can not create or open this file!"));
			return;
		}
		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_5_7);
		out << quint32(0x66601000);
		out << pieceSave.size() + pieceSaveTmp.size();//子个数
		out << pieceSave.size();
		auto tmp = pieceSave;
		while (!tmp.empty())
		{
			auto t = tmp.front();
			tmp.pop_front();
			out << t.second;
		}
		tmp = pieceSaveTmp;
		while (!tmp.empty())
		{
			auto t = tmp.back();
			tmp.pop_back();
			out << t.second;
		}
		logWidget->append(path);
		logWidget->append("");
		logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
	}
}

void Haixin::loadFile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("Haixin Save File(*.sav)"));
	if (path.length() != 0)
	{
		newFile();
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::critical(this, tr("Error"), tr("Can not open the file"));
			return;
		}
		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_5_7);
		quint32 ver;
		in >> ver;

		if (ver != quint32(0x66601000))
		{
			auto vers = QString::number(ver);
			if (vers[0] == vers[1] && vers[1] == vers[2] && vers[2] == '6')
				QMessageBox::critical(nullptr,
					QObject::tr("Error"),
					QObject::tr("Failed to load this save file.\nThis file was created by Haixin Board v%1.%2.")
					.arg((vers[3] == '0' ? "" : QString(vers[3])) + vers[4]).arg(QString(vers[5]) + QString(vers[6]) + QString(vers[7])));
			else
				QMessageBox::critical(nullptr,
					QObject::tr("Error"),
					QObject::tr("Failed to load this save file.\nThis file was not created by Haixin Board."));
			return;
		}

		pieceSave.clear();
		pieceSaveTmp.clear();
		int all, loc;
		in >> all >> loc;
		engine->setBlack(PLAYER);
		engine->setWhite(PLAYER);
		for (int i = 0; i < all; i++)
		{
			QPoint p;
			in >> p;
			engine->drop(QPoint(p.y() + 1, p.x() + 1), i % 2 == 0 ? Piece_Black : Piece_White);
			drop(p.x(), p.y(), i % 2 == 0 ? Piece_Black : Piece_White);
		}
		for (int i = all - 1; i >= loc; i--)previous_Step();
		engine->setBlack(settings["black"] == "COM" ? COM : PLAYER);
		engine->setWhite(settings["white"] == "COM" ? COM : PLAYER);

		logWidget->append(path);
		logWidget->append("");
		logWidget->verticalScrollBar()->setSliderPosition(logWidget->verticalScrollBar()->maximum());
	}
}