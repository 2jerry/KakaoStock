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
	
	/*dataload dataThread1("»ï¼ºÀüÀÚ");
	dataload dataThread2("ÄÚ½ºÇÇ");
	dataload dataThread3("ÄÚ½º´Ú");
	dataThread1.start();/*
	dataThread2.start();
	dataThread3.start();*/


	/*dataload *dataThread[2];
	for (int i = 0; i < 2; i++)
	{
		dataThread[i] =  new dataload();
	}
	dataThread[0]->htmlLoadName = "ÄÚ½º´Ú";
	dataThread[1]->htmlLoadName = "ÄÚ½ºÇÇ";
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
