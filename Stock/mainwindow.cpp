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
	height = info[0].tradingVol->height();
	width = info[0].tradingVol->width();
	// �ֽ� ���� ���
	int chk = -1;
	ifstream ifs;
	string line;

	QString saveName[7] = { qkor("�Ｚ����"),qkor("�ڽ���"),
		qkor("�ڽ���"),qkor("�Ｚ���̿�"),
		qkor("KODEX ��������"),qkor("KODEX �ι���"),qkor("��Ʈ����") };

	for (int i = 0; i < 7; i++)
	{
		//info[i].setTxt(saveName[i]); // �ֽ� ���� �ҷ�����


		//info[i].stockName->setText(saveName[i]);

	}

}

/* --- event function --- */
void MainWindow::on_bnt_clicked(int i)
{
	if (i == 1)
	{
		// ���ο� â ����
		add.setUpWidget();
		//  add.cnt = 0;


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

	


	ofs << "thread " <<_idx<<" "<< dataThread[_idx]->htmlLoadName << "is finished.\n";

	//if (dataThread[_idx]->htmlLoadName == "�ڽ��� ����" || dataThread[_idx]->htmlLoadName == "�ڽ��� ����")
	//	dataThread[_idx]->KOSPI_KOSDAQ_Parser(dataThread[_idx]->htmlLoadName);
	//else
	//	dataThread[_idx]->stock_Parser(dataThread[_idx]->htmlLoadName);
	/*info[_idx].tradingVol->setText("");
	info[_idx].yesterdayPrice->setText("");
	info[_idx].currentPrice->setText("");
	info[_idx].exchangeRate->setText("");*/
	x = info[_idx].tradingVol->x();
	y = info[_idx].tradingVol->y();
	if (dataThread[_idx]->isFinished() == false)
		return;
	if (dataThread[_idx]->isRunning() == true)
		return;
	if (tmp_trade[_idx] != dataThread[_idx]->info["�ŷ���"] || tmp_cur[_idx] != dataThread[_idx]->info["���簡"])
	{
		
		
		ofs << tmp_trade[_idx] << " : " << dataThread[_idx]->info["�ŷ���"] << "\n";
		ofs << tmp_cur[_idx] << " : " << dataThread[_idx]->info["���簡"] << "\n";
		if (dataThread[_idx]->flag < 0)
		{
			info[_idx].currentPrice->setStyleSheet("color:rgb(000,102,255)");
			info[_idx].yesterdayPrice->setStyleSheet("color:rgb(000,102,255)");
			info[_idx].exchangeRate->setStyleSheet("color:rgb(000,102,255)");
		}
		info[_idx].stockName->setText(qstr(dataThread[_idx]->htmlLoadName));
		info[_idx].tradingVol->setText(qstr(dataThread[_idx]->info["�ŷ���"]));
		info[_idx].yesterdayPrice->setText(qstr(dataThread[_idx]->info["���Ϻ�"]));
		info[_idx].currentPrice->setText(qstr(dataThread[_idx]->info["���簡"]));
		info[_idx].exchangeRate->setText(qstr(dataThread[_idx]->info["�����"]));
	}
	else
	{
		return;
	}

	tmp_trade[_idx] = dataThread[_idx]->info["�ŷ���"];
	tmp_cur[_idx] = dataThread[_idx]->info["���簡"];
	/* ������� �ҷ��� �ֽ� */

	info[_idx].stockName->setFont(font);



	/*	for (auto j = dataThread[_idx]->info.begin(); j != dataThread[_idx]->info.end(); ++j)
		{


			ofs << j->first << " : " << j->second << "\n";
		}*/

	ofs.close();
	this->update();
}

void MainWindow::on_timer_count()
{

	for (int i = 0; i < 7; i++)
	{
		dataThread[i]->start(); // ������ ����
		dataThread[i]->idx = i;
	}

}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter paint(this);
	//paint.drawRect(x, y, width, height);
	paint.end();
}
