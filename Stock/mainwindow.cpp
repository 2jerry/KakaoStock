#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qstring.h> 
#include <qtextcodec.h>
#include <iostream>
#include <fstream>
#include <string>
#include "QSignalMapper"

QString qstr(std::string _str)
{
	return qkor(_str.c_str());

}



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QSignalMapper *signalMapper = new QSignalMapper(this);

	for (int i = 0; i < 3; i++)
	{
		QObject::connect(ui->btn[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(ui->btn[i], i);
		QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_bnt_clicked(int)));
	}
	for (int i = 0; i<7; i++)
	{
		dataThread[i] = new dataload(stockName[i]);
		connect(dataThread[i], SIGNAL(FinishCount(int)), this, SLOT(on_thread_finish(int)));
		//dataThread[i]->start();
	}

	/*
	for (int i = 0; i<8; i++)
	{
	//info[i].stock = new QGroupBox(ui->centralWidget);
	// info[i].stockSetup(info[i].stock,"color:red;",i);

	add.info[i].stock = new QGroupBox(ui->centralWidget);
	add.info[i].stockSetup(add.info[i].stock, "color:red;", i);
	}
	*/

}

void MainWindow::set_window()
{
	for (int i = 0; i < 7; i++)
	{
		info[i].stock = new QGroupBox(ui->centralWidget);
		info[i].stockSetup(info[i].stock, "color:red;", i);
	}

	// 주식 정보 출력
	int chk = -1;
	ifstream ifs;
	string line;

	QString saveName[7] = { qkor("삼성전자"),qkor("코스피"),
		qkor("코스닥"),qkor("삼성바이오"),
		qkor("KODEX 레버리지"),qkor("KODEX 인버스"),qkor("셀트리온") };

	for (int i = 0; i < 7; i++)
	{
		//info[i].setTxt(saveName[i]); // 주식 정보 불러오기


		info[i].stockName->setText(saveName[i]);

	}
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_bnt_clicked(int i)
{
	if (i == 1)
	{
		// 새로운 창 띄우기
		add.setUpWidget();
		//  add.cnt = 0;


	}

	cnt++;
	//info[cnt/3].stockName->setText(add.saveName[0]);
	//info[cnt/3].setTxt((add.saveName[0]),QString::number(2514000),QString::number(200362),QString::number(28000),QString::number(1.13));
	// ui->btn[i]->setText(QString("%1 ").arg(i) + "button");
}

void MainWindow::on_thread_finish(const int _cnt)
{
	std::ofstream ofs;
	ofs.open("thread_result.txt");
	QFont font;
	font.setFamily("D2coding");
	font.setPointSize(13);
	for (int i = 0; i < 7; i++)
	{
		if (dataThread[i]->isFinished() == false)
			return;
		if (dataThread[i]->isRunning() == true)
			return;

		/* 쓰레드로 불러온 주식 */
		ofs << "thread " << dataThread[i]->htmlLoadName << "is finished.\n";
		info[i].stockName->setFont(font);
		if(dataThread[i]->flag < 0)
		{
			info[i].currentPrice->setStyleSheet("color:blue");
			info[i].yesterdayPrice->setStyleSheet("color:blue");
			info[i].exchangeRate->setStyleSheet("color:blue");
		}
		info[i].stockName->setText(qstr(dataThread[i]->htmlLoadName));
		info[i].tradingVol->setText(qstr(dataThread[i]->info["거래량"]));
		info[i].yesterdayPrice->setText(qstr(dataThread[i]->info["전일비"]));
		info[i].currentPrice->setText(qstr(dataThread[i]->info["현재가"]));
		info[i].exchangeRate->setText(qstr(dataThread[i]->info["등락률"]));
		for (auto j = dataThread[i]->info.begin(); j != dataThread[i]->info.end(); ++j)
		{


			ofs << j->first << " : " << j->second << "\n";
		}

	}
	ofs.close();
}