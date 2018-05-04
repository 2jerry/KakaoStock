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
	std::string stockName[7] =	{ "�ڽ���","�ڽ���", "�Ｚ����","�Ｚ���̿�","KODEX ��������",\
		"KODEX �ι���","��Ʈ����"};
	
private:
	Ui::StockClass ui;

private slots:
	void on_thread_finish(int i);
};
