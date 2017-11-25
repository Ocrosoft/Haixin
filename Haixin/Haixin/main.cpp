#include "haixin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Haixin w;
	w.show();
	return a.exec();
}
