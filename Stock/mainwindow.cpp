#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qstring.h> 
#include <qtextcodec.h>
#include <iostream>
#include <fstream>
#include <string>
#include <QPainter>
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
	m_timer = new QTimer;
	connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timer_count()));

	QSignalMapper *signalMapper = new QSignalMapper(this);

	for (int i = 0; i < 3; i++)
	{
		QObject::connect(ui->btn[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(ui->btn[i], i);
		QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_bnt_clicked(int)));
	}
	for (int i = 0; i < 7; i++)
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
	m_timer->start(500);
}

MainWindow::~MainWindow()
{
	delete ui;
}

/* --- user function ---*/
void MainWindow::set_window()
{
	for (int i = 0; i < 7; i++)
	{
		info[i].stock = new QGroupBox(ui->centralWidget);
		//info[i].stock->setStyleSheet("background:rgb(255,255,255)");
		info[i].stockSetup(info[i].stock, "color:rgb(204,0,0);", i);
		//info[i].stock->setStyleSheet("border: none");
		
	}
	// 주식 정보 출력
	int chk = -1;
	ifstream ifs;
	string line;

	QString saveName[7] = { qkor("삼성전자"),qkor("코스피"),
		qkor("코스닥"),qkor("삼성바이오"),
		qkor("KODEX 레버리지"),qkor("KODEX 인버스"),qkor("셀트리온") };

}

/* --- event function --- */
void MainWindow::on_bnt_clicked(int i)
{
	switch (i)
	{
	case 1:
		// 새로운 창 띄우기
		add.setUpWidget();
		//  add.cnt = 0;
		break;
	case 2:
		align.setUpWidget();
		break;
	case 3:
		break;
	default:
		break;
	}
	
		


	cnt++;
	//info[cnt/3].stockName->setText(add.saveName[0]);
	//info[cnt/3].setTxt((add.saveName[0]),QString::number(2514000),QString::number(200362),QString::number(28000),QString::number(1.13));
	// ui->btn[i]->setText(QString("%1 ").arg(i) + "button");
}

void MainWindow::on_thread_finish(const int _idx)
{
	std::ofstream ofs;
	ofs.open("thread_result.txt", ios::out | ios::app);
	QFont font;
	font.setFamily(QString::fromUtf8("\353\247\221\354\235\200 \352\263\240\353\224\225"));
	font.setPointSize(11);
	font.setBold(false);


	
	if (dataThread[_idx]->isFinished() == false)
		return;
	if (dataThread[_idx]->isRunning() == true)
		return;
	if (tmp_trade[_idx] != dataThread[_idx]->info["거래량"] || tmp_cur[_idx] != dataThread[_idx]->info["현재가"])
	{
		ofs << "thread " << _idx <<dataThread[_idx]->htmlLoadName << "is finished.\n";

		ofs << tmp_trade[_idx] << " : " << dataThread[_idx]->info["거래량"] << "\n";
		ofs << tmp_cur[_idx] << " : " << dataThread[_idx]->info["현재가"] << "\n";
		if (dataThread[_idx]->flag < 0)
		{
			info[_idx].currentPrice->setStyleSheet("color:rgb(000,102,255)");
			info[_idx].yesterdayPrice->setStyleSheet("color:rgb(000,102,255)");
			info[_idx].exchangeRate->setStyleSheet("color:rgb(000,102,255)");
			info[_idx].frame->setStyleSheet("color:rgb(000,102,255)");
		}
		else if (dataThread[_idx]->flag == 0)
		{
			info[_idx].currentPrice->setStyleSheet("color:rgb(000,000,000)");
			info[_idx].yesterdayPrice->setStyleSheet("color:rgb(000,000,000)");
			info[_idx].exchangeRate->setStyleSheet("color:rgb(000,000,000)");
		}
		info[_idx].stockName->setText(qstr(dataThread[_idx]->htmlLoadName));
		info[_idx].tradingVol->setText(qstr(dataThread[_idx]->info["거래량"]));
		info[_idx].yesterdayPrice->setText(qstr(dataThread[_idx]->info["전일비"]));
		info[_idx].currentPrice->setText(qstr(dataThread[_idx]->info["현재가"]));
		info[_idx].exchangeRate->setText(qstr(dataThread[_idx]->info["등락률"]));
		
		tmp_trade[_idx] = dataThread[_idx]->info["거래량"];
		tmp_cur[_idx] = dataThread[_idx]->info["현재가"];

		info[_idx].frame->setVisible(true);
		//y = info[_idx].currentPrice->y() + _idx * 51;
		//this->update();
		
	}
	else
	{
		return;
	}

	info[_idx].stockName->setFont(font);
	ofs.close();

}

void MainWindow::on_timer_count()
{
	for (int i = 0; i < 7; i++)
	{
	/*	std::string n = stockName[i] + ".html";
		std::remove(n.c_str());

	*/
		if (align.m == 1)
		{

		}
		info[i].frame->setVisible(false);
		dataThread[i]->start(); // 쓰레드 시작
		dataThread[i]->idx = i;
		
	}

}

