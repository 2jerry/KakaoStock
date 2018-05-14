#ifndef STOCKINFO_H
#define STOCKINFO_H
#include "dataload.h"

#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

#include <QtCore/QVariant>
#include <QtWidgets/QWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QSignalMapper>

class stockInfo 
{
public:
	stockInfo();
	void setTxt(QString saveName);
	void stockSetup(QGroupBox *stock, QString txtcolor, int cnt);
	QString numToShow(QString number);

public:
	QGroupBox * stock;
	QLabel *tradingVol;              // 거래량
	QLabel *currentPrice;           // 현재가
	QLabel *yesterdayPrice;         // 전일비
	QLabel *exchangeRate;           // 등락률
	QCommandLinkButton *stockName; // 주식이름
	QString *txtColor;

};

#endif // STOCKINFO_H
