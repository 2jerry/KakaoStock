#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QSignalMapper"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QSignalMapper *signalMapper = new QSignalMapper(this);

	for (int i = 0; i<3; i++)
	{


		QObject::connect(ui->btn[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(ui->btn[i], i);
		QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_bnt_clicked(int)));
	}

	for (int i = 0; i<8; i++)
	{
		//info[i].stock = new QGroupBox(ui->centralWidget);
		// info[i].stockSetup(info[i].stock,"color:red;",i);

		add.info[i].stock = new QGroupBox(ui->centralWidget);
		add.info[i].stockSetup(add.info[i].stock, "color:red;", i);
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

