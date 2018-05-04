#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Stock.h"
#include "dataload.h"


class Stock : public QMainWindow
{
	Q_OBJECT

public:
	Stock(QWidget *parent = Q_NULLPTR);
	void startThread();
	dataload *dataThread[7];
	std::string stockName[7] =	{ "코스피","코스닥", "삼성전자","삼성바이오","KODEX 레버리지",\
		"KODEX 인버스","셀트리온"};
	
private:
	Ui::StockClass ui;

private slots:
	void on_thread_finish(int i);
};
