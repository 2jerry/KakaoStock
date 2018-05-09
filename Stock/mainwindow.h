#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>

#include "dataload.h"
#include "stockinfo.h"
#include "addwidget.h"

#define qkor(str) QString::fromLocal8Bit(str)
//#define qstr(str) QString::fromStdString(str)
#define EDIT 0
#define ADD 1
#define ALIGN 2

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void set_window();
	addWidget add;
	stockInfo info[7]; // 주식 7개 
	void startThread();
	dataload *dataThread[7]; // 쓰레드의 info 안에 정보 들어가 익음
	std::string stockName[7] = { "코스피","코스닥", "삼성전자","삼성바이오","KODEX 레버리지",\
		"KODEX 인버스","셀트리온" };

public slots:
	void on_bnt_clicked(int i);

private:
	int mode;
	int cnt = -1;
	int box_cnt = 0;
	Ui::MainWindow *ui;


private slots:
	void on_thread_finish(const int _cnt);
};

#endif // MAINWINDOW_H
#pragma once
