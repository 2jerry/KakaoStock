#include "stockinfo.h"
#include "mainwindow.h"

#include <iostream>
#include <fstream>
#include <string>


//#define qstr(str) QString::fromStdString(str)

stockInfo::stockInfo()
{

	//ss.startThread();
}
QString stockInfo::numToShow(QString number)
{

	for (int i = number.length(); i > 0; i = i - 3)
	{
		if (i == number.length()) continue;
		else
			number.insert(i, ',');
	}
	return number;
}

void stockInfo::setTxt(QString saveName)
{
	int color_flag = 0;
	ifstream ifs;
	string line;
	QFont font;
	font.setFamily("D2coding");
	font.setPointSize(15);

	for (int i = 0; i < 7; i++)
	{
		QString fl = saveName + ".txt";
		ifs.open(fl.toLocal8Bit().constData(), std::ios::in); // 주식 파일 읽기

		getline(ifs, line); // 거래량
		//tradingVol->setText(qstr(line));
		//tradingVol->setText(qstr(ss.dataThread[i]->info["거래량"]));
		getline(ifs, line); // 등락률
		//exchangeRate->setText(qstr(line));
		//exchangeRate->setText(qstr(ss.dataThread[i]->info["등락률"]));
		if (line.find("-"))
			color_flag = 1;
		getline(ifs, line); // 전일비
		//yesterdayPrice->setText(qstr(line));

		if (color_flag)
		{
			currentPrice->setStyleSheet("color:red");
			yesterdayPrice->setStyleSheet("color:red");
			exchangeRate->setStyleSheet("color:red");
		}
		else
		{
			currentPrice->setStyleSheet("color:blue");
			yesterdayPrice->setStyleSheet("color:blue");
			exchangeRate->setStyleSheet("color:blue");

		}
		//stockName->setFont(font);
		//stockName->setText(saveName); // 선택한 주식 이름 출력
		ifs.close();
	}

}


// 메인 윈도우에 주식 정보 넣는 라벨 초기화 함수
void stockInfo::stockSetup(QGroupBox *stock, QString txtcolor, int cnt)
{
	stock->setGeometry(QRect(0, 32 + 51 * (cnt), 330, 51));

	QFont font;
	font.setFamily("D2coding");

	font.setPointSize(10);
	tradingVol = new QLabel(stock);
	tradingVol->setGeometry(QRect(160, 24, 91, 20));
	tradingVol->setFont(font);
	tradingVol->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

	yesterdayPrice = new QLabel(stock);
	yesterdayPrice->setGeometry(QRect(220, 24, 91, 20));
	yesterdayPrice->setFont(font);
	yesterdayPrice->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
	yesterdayPrice->setStyleSheet(txtcolor);

	exchangeRate = new QLabel(stock);
	exchangeRate->setGeometry(QRect(220, 7, 91, 20));
	exchangeRate->setFont(font);
	exchangeRate->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
	exchangeRate->setStyleSheet(txtcolor);

	font.setPointSize(13);
	font.setBold(true);
	font.setWeight(75);

	currentPrice = new QLabel(stock);
	currentPrice->setGeometry(QRect(160, 7, 91, 20));
	currentPrice->setFont(font);
	currentPrice->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
	currentPrice->setFocusPolicy(Qt::StrongFocus);
	currentPrice->setStyleSheet(txtcolor);


	stockName = new QCommandLinkButton(stock);

	stockName->setGeometry(QRect(0, 3, 361, 61));
	stockName->setFont(font);
	//stockName->setFocusPolicy(Qt::StrongFocus);
	stockName->setIconSize(QSize(0, 0));
}

