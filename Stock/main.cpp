#include "dataload.h"
#include "Stock.h"
#include <QtWidgets/QApplication>
#include <Windows.h>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Stock w;
	int t = 0;
	while (t<20)
	{

		w.startThread();
		Sleep(60000);
		t++;
	}
	
	/*dataload dataThread1("�Ｚ����");
	dataload dataThread2("�ڽ���");
	dataload dataThread3("�ڽ���");
	dataThread1.start();/*
	dataThread2.start();
	dataThread3.start();*/


	/*dataload *dataThread[2];
	for (int i = 0; i < 2; i++)
	{
		dataThread[i] =  new dataload();
	}
	dataThread[0]->htmlLoadName = "�ڽ���";
	dataThread[1]->htmlLoadName = "�ڽ���";
	dataThread[0]->start();
	dataThread[1]->start();*/
	


	/*for (int i = 0;i<5;i++)
	{
		dataThread->start();
		Sleep(2000);
	}*/
		
//	qApp->quit();
	w.show();
	return a.exec();


}
