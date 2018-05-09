
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.set_window();
	w.show();
	int cnt = 0;
	for (int i = 0; i < 7; i++)
		w.dataThread[i]->start();

	//Stock s;
	//int t = 0;
	//s.startThread();
	return a.exec();
}
