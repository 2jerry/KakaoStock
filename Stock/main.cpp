
#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.set_window();
	w.show();
	int cnt = 0;
	//for (int i = 0; i < 7; i++)
	//{
	//	w.dataThread[i]->start();
	//	w.dataThread[i]->idx = i;
	//	//w.info[i].frame->setVisible(false);
	//}
	
	//while (cnt < 100)
	//{

	//
	//	Sleep(300); //0.3ÃÊ
	//	cnt++;
	//}

	//Stock s;
	//int t = 0;
	//s.startThread();
	return a.exec();
}
