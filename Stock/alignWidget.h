#pragma once
#ifndef __ALIGNWIDGET_H__
#define __ALIGNWIDGET_H__

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QObject>
#include <qsignalmapper.h>
//#include "mainwindow.h"

class alignWidget :public QWidget
{
	Q_OBJECT
public:
	alignWidget();
	~alignWidget();
	QWidget *window = new QWidget();
	QPushButton *btn[5];
	QString qkor(const char *str);
	QString btn_name[5] = { qkor("정렬안함"),qkor("등락률"), qkor("등락"),\
		qkor("거래량"), qkor("가격") };
	int m;
	void setUpWidget()
	{
		std::locale::global(std::locale("kor"));
		window->resize(186, 187);
		window->setWindowTitle(qkor("추가"));

		window->show();
	}

public slots:
	void button_clicked(int i);
};


#endif // !ALIGNWIDGET_H__



