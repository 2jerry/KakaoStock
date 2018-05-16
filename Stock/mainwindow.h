#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QTimer>
#include <QVector>

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
	dataload *dataThread[7]; // 쓰레드의 info 안에 정보 들어가 익음
	std::string stockName[7] = { "코스피 지수","코스닥 지수", "삼성전자","삼성바이오로직스","KODEX 레버리지",\
		"KODEX 인버스","셀트리온" };
	std::string tmp_trade[7] = {};
	std::string tmp_cur[7] = {};
	int x, y,height,width;
	int idx;

private:
	int mode;
	int cnt = -1;
	int box_cnt = 0;
	QTimer *m_timer;
	Ui::MainWindow *ui;
	

public slots:
	void on_bnt_clicked(int i);
	
private slots:
	void on_thread_finish(const int _cnt);
	void on_timer_count();
	void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
#pragma once
