#include "stockinfo.h"
stockInfo::stockInfo()
{

}
QString stockInfo::numToShow(QString number)
{

	for (int i = number.length(); i>0; i = i - 3)
	{
		if (i == number.length()) continue;
		else
			number.insert(i, ',');
	}
	return number;
}
void stockInfo::setTxt(QString name, QString current, QString trading, QString yesterday, QString exchange)
{
	std::locale::global(std::locale("kor"));
	QFont font;

	tradingVol->setText(numToShow(trading));
	currentPrice->setText(numToShow(current));
	yesterdayPrice->setText(numToShow(yesterday));
	exchangeRate->setText(exchange + "%");

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
	yesterdayPrice->setGeometry(QRect(220, 7, 91, 20));
	yesterdayPrice->setFont(font);
	yesterdayPrice->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
	yesterdayPrice->setStyleSheet(txtcolor);

	exchangeRate = new QLabel(stock);
	exchangeRate->setGeometry(QRect(220, 24, 91, 20));
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

	stockName->setGeometry(QRect(0, 4, 361, 61));
	stockName->setFont(font);
	stockName->setFocusPolicy(Qt::StrongFocus);
	stockName->setIconSize(QSize(0, 0));
}

