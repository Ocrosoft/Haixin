#ifndef HAIXIN_H
#define HAIXIN_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QVector>
#include <QQueue>
#include <QStack>
#include <QMap>

#define  Piece_Black 0
#define  Piece_White 1
const QPoint invalidPoint = QPoint(-1, -1);

class QTextEdit;
class QMenu;
class QToolBar;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsItem;
class QGraphicsPixmapItem;
class QGraphicsTextItem;
class HaixinEngine;
class Haixin : public QMainWindow
{
	Q_OBJECT

public:
	Haixin(QWidget  *parent = 0);
	~Haixin();
	void init();
	void setSettings(QMap<QString, QString> settings);
	void receiveAPP(QApplication *a);

private slots:
	void HaixinAbout();//关于消息框
	void previous_Step();
	void next_Step();
	void first_Step();
	void final_Step();
	void toEnglish();
	void toChinese();
	void computerChange();
	void commandInput();
	bool eventFilter(QObject *obj, QEvent *e);
	void setNumberVisible();//显示数字
	void setLogVisible();//显示日志
	void setAnalysesVisible();//显示分析
	void newFile();//新建
	void saveFile();//保存
	void loadFile();//载入

private:
	QTextEdit *logWidget;//消息输出窗口
	QTextEdit *cmdWidget;//命令输入窗口
	QMenu *menu_game;//菜单-游戏
	QMenu *menu_setting;//菜单-设置
	QMenu *menu_display;//菜单-显示
	QMenu *menu_help;//菜单-帮助
	QToolBar *toolBar_operation;//工具栏-操作
	QGraphicsScene *board;//棋盘
	QGraphicsView *boardView;
	QGraphicsPixmapItem* boardStatu[20][20];//棋盘状态
	QStack<QPair<QPair<QGraphicsPixmapItem*, QGraphicsTextItem*>,QPoint> > pieceSave;//步骤保存
	QStack<QPair<QPair<QGraphicsPixmapItem*, QGraphicsTextItem*>, QPoint> > pieceSaveTmp;//回退时保存后步
	HaixinEngine *engine;
	int cvcing = 0;
	QMap<QString, QString> settings;

	void drawGUI();
	void drop(int x, int y, int type);
	void closeEvent(QCloseEvent *event);
	QApplication *a;

protected:
	void paintEvent(QPaintEvent  *event);//图形绘制
	void mousePressEvent(QMouseEvent  *event);//鼠标单击
};

#endif // HAIXIN_H
