#pragma once
#ifndef __STOCK_H__
#define __STOCK_H__


#include <QtWidgets/QMainWindow>
#include "ui_Stock.h"
#include "dataload.h"

class Stock : public QMainWindow
{
	Q_OBJECT

public:
	//Stock(QWidget *parent = Q_NULLPTR);
	void startThread();
	dataload *dataThread[7];
	std::string stockName[7] =	{ "코스피","코스닥", "삼성전자","삼성바이오","KODEX 레버리지",\
		"KODEX 인버스","셀트리온"};
	//MainWindow w;


private slots:
	//void on_thread_finish(const std::map<std::string, std::string> _info);
	void on_thread_finish(const int _cnt);
};
#endif // !__STOCK_H__