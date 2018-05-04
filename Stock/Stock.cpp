#include "Stock.h"

Stock::Stock(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	int t = 0;
	/*dataThread[0] = new dataload("KODEX 인버스");
	connect(dataThread[0], SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish(i)));
	dataThread[0]->start();*/
	//dataThread[1] = new dataload("코스피");
	
	
		
	
	

	//connect(dataThread1, SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish));
	//connect(dataThread2, SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish));

	//dataThread1->start();
	//dataThread2->start();


}
void Stock::startThread()
{
	for (int i = 0; i<7; i++)
	{
		dataThread[i] = new dataload(stockName[i]);
		connect(dataThread[i], SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish(i)));
		dataThread[i]->start();
	}
}
void Stock::on_thread_finish(int i)
{
	std::ofstream ofs;
	ofs.open("thread_result.txt", std::ios::out | std::ios::app);
	if (dataThread[i]->isFinished() == false)
		return;
	if (dataThread[i]->isRunning() == true)
		return;

	ofs << "thread " << i << "is finished.";


}
