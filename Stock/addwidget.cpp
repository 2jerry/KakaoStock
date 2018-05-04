#include "addwidget.h"
#include <QSignalMapper>
#include <QTextCodec>

QString addWidget::qkor(const char *str)
{
	return QString::fromLocal8Bit(str);

}



addWidget::addWidget()
{
	std::locale::global(std::locale("kor"));
	QSignalMapper *signalMapper = new QSignalMapper(this);
	for (int i = 0; i<BOX_SZ; i++)
	{
		check[i] = new QCheckBox();
		check[i]->setText(name[i]);
		QObject::connect(check[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(check[i], i);
		QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(checkBox_clicked(int)));

		layout2->addWidget(check[i]);
	}



}


addWidget::~addWidget()
{

}


void addWidget::checkBox_clicked(int i)
{

	// cnt = cnt/18;
	if (check[i]->isChecked() == false)
	{

		//cnt = cnt/18;
		// cnt--;
		// label->setText(" Select "+name[i]);

		saveName[i] = "";
	}
	else
	{
		//cnt++;
		label->setText(" Select " + name[i]);
		//nameFlag[i] = i;
		saveName[i] = name[i];
	}


}

// ���� �Ǿ� �ִ� 
void addWidget::okButton_clicked()
{
	window->close();
	int chk = -1;
	ifstream ifs;
	string line;
	QFont font;
	font.setFamily("D2coding");
	font.setPointSize(15);

	for (int i = 0; i<BOX_SZ; i++) // üũ �Ǿ� �ִ� �ֵ� ����ϱ� -> �ؽ����� ��������
	{
		if (check[i]->isChecked()) // üũ�� �Ǿ� �ִ� �ֵ鸸 ���
		{
			chk++;
			check[i]->setChecked(false);
			QString fl = saveName[i] + ".txt";
			ifs.open(fl.toLocal8Bit().constData(), std::ios::in); // �ֽ� ���� �б�
			getline(ifs, line); // ���簡
			info[i].currentPrice->setText(QString::fromStdString(line));
			getline(ifs, line); // ���Ϻ�
			info[i].yesterdayPrice->setText(QString::fromStdString(line));
			getline(ifs, line); // �ŷ���
			info[i].tradingVol->setText(QString::fromStdString(line));
			getline(ifs, line); // �����
			info[i].exchangeRate->setText(QString::fromStdString(line));

			if (line.find("-"))
			{
				info[i].currentPrice->setStyleSheet("color:red");
				info[i].yesterdayPrice->setStyleSheet("color:red");
				info[i].exchangeRate->setStyleSheet("color:red");
			}
			else
			{
				info[i].currentPrice->setStyleSheet("color:blue");
				info[i].yesterdayPrice->setStyleSheet("color:blue");
				info[i].exchangeRate->setStyleSheet("color:blue");

			}

			info[i].stockName->setFont(font);
			info[i].stockName->setText(saveName[i]); // ������ �ֽ� �̸� ���
			ifs.close();
		}

	}

}
