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
	stockInfo info[7]; // �ֽ� 7�� 
	dataload *dataThread[7]; // �������� info �ȿ� ���� �� ����
	std::string stockName[7] = { "�ڽ��� ����","�ڽ��� ����", "�Ｚ����","�Ｚ���̿�������","KODEX ��������",\
		"KODEX �ι���","��Ʈ����" };
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
