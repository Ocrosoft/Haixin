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
	void HaixinAbout();//������Ϣ��
	void previous_Step();
	void next_Step();
	void first_Step();
	void final_Step();
	void toEnglish();
	void toChinese();
	void computerChange();
	void commandInput();
	bool eventFilter(QObject *obj, QEvent *e);
	void setNumberVisible();//��ʾ����
	void setLogVisible();//��ʾ��־
	void setAnalysesVisible();//��ʾ����
	void newFile();//�½�
	void saveFile();//����
	void loadFile();//����

private:
	QTextEdit *logWidget;//��Ϣ�������
	QTextEdit *cmdWidget;//�������봰��
	QMenu *menu_game;//�˵�-��Ϸ
	QMenu *menu_setting;//�˵�-����
	QMenu *menu_display;//�˵�-��ʾ
	QMenu *menu_help;//�˵�-����
	QToolBar *toolBar_operation;//������-����
	QGraphicsScene *board;//����
	QGraphicsView *boardView;
	QGraphicsPixmapItem* boardStatu[20][20];//����״̬
	QStack<QPair<QPair<QGraphicsPixmapItem*, QGraphicsTextItem*>,QPoint> > pieceSave;//���豣��
	QStack<QPair<QPair<QGraphicsPixmapItem*, QGraphicsTextItem*>, QPoint> > pieceSaveTmp;//����ʱ�����
	HaixinEngine *engine;
	int cvcing = 0;
	QMap<QString, QString> settings;

	void drawGUI();
	void drop(int x, int y, int type);
	void closeEvent(QCloseEvent *event);
	QApplication *a;

protected:
	void paintEvent(QPaintEvent  *event);//ͼ�λ���
	void mousePressEvent(QMouseEvent  *event);//��굥��
};

#endif // HAIXIN_H
