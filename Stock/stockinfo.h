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
	QLabel *tradingVol;              // �ŷ���
	QLabel *currentPrice;           // ���簡
	QLabel *yesterdayPrice;         // ���Ϻ�
	QLabel *exchangeRate;           // �����
	QCommandLinkButton *stockName; // �ֽ��̸�
	QString *txtColor;

};

#endif // STOCKINFO_H
