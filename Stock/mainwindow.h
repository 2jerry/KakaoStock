#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define EDIT 0
#define ADD 1
#define ALIGN 2

#include "stockinfo.h"
#include <QMainWindow>
#include "addwidget.h"
#include <QSignalMapper>
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
	stockInfo stock_info[8];


	public slots:
	void on_bnt_clicked(int i);

private:
	Ui::MainWindow *ui;
	int mode;
	int cnt = -1;
	int box_cnt = 0;
};

#endif // MAINWINDOW_H
#pragma once
