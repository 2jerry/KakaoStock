#include "Stock.h"

//Stock::Stock(QWidget *parent)
//	: QMainWindow(parent)
//{
//	ui.setupUi(this);
//
//	int t = 0;
//	/*dataThread[0] = new dataload("KODEX 인버스");
//	connect(dataThread[0], SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish(i)));
//	dataThread[0]->start();*/
//	//dataThread[1] = new dataload("코스피");
//	
//	
//		
//	
//	
//
//	//connect(dataThread1, SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish));
//	//connect(dataThread2, SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish));
//
//	//dataThread1->start();
//	//dataThread2->start();
//
//
//}
void Stock::startThread()
{
	for (int i = 0; i<7; i++)
	{
		dataThread[i] = new dataload(stockName[i]);
		connect(dataThread[i], SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish(int)));
		dataThread[i]->start();
	}
}
void Stock::on_thread_finish(const int cnt)
{
	w.set_window();
	std::ofstream ofs;
	ofs.open("thread_result.txt");
	for (int i = 0; i < cnt; i++)
	{
		if (dataThread[i]->isFinished() == false)
			return;
		if (dataThread[i]->isRunning() == true)
			return;
		
		/* 쓰레드로 불러온 주식 */
		ofs << "thread " << dataThread[i]->htmlLoadName << "is finished.\n";
		w.info[i].stockName->setText(qstr(dataThread[i]->htmlLoadName));
		w.info[i].tradingVol->setText(qstr(dataThread[i]->info["거래량"]));
		w.info[i].exchangeRate->setText(qstr(dataThread[i]->info["등락률"]));
		w.info[i].yesterdayPrice->setText(qstr(dataThread[i]->info["전일비"]));
		w.info[i].currentPrice->setText(qstr(dataThread[i]->info["현재가"]));
		for (auto j = dataThread[i]->info.begin(); j != dataThread[i]->info.end(); ++j)
		{
			

			ofs << j->first << " : " << j->second << "\n";
		}
		
	}
	w.show();
	
	ofs.close();
}
