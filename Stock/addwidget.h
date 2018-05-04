#ifndef ADDWIDGET_H
#define ADDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QCheckBox>
#include <QObject>

#include <iostream>
#include <fstream>
#include <string>

#include "stockinfo.h"
#define BOX_SZ 7

using namespace std;

class addWidget :public QWidget
{	Q_OBJECT

public:
	QWidget * window = new QWidget();
	QVBoxLayout *verticalLayout = new QVBoxLayout(window);
	QLineEdit *lineEdit = new QLineEdit(window);
	QLabel *label = new QLabel(window);
	QTabWidget * tabWidget = new QTabWidget(window);
	QWidget *viewTab = new QWidget();
	QGridLayout *gridLayout = new QGridLayout(viewTab);
	QScrollArea *scrollArea = new QScrollArea(viewTab);
	QWidget *scrollAreaWidgetContents = new QWidget();
	QWidget *listTab = new QWidget();
	QGridLayout *gridLayout_2 = new QGridLayout(listTab);
	QScrollArea *scrollArea_2 = new QScrollArea(listTab);
	QWidget *scrollAreaWidgetContents_2 = new QWidget();
	QPushButton *pushButton = new QPushButton(window);

	QVBoxLayout *layout1 = new QVBoxLayout(scrollAreaWidgetContents);
	QVBoxLayout *layout2 = new QVBoxLayout(scrollAreaWidgetContents_2);
	QCheckBox *check[BOX_SZ];
	QString name[BOX_SZ] = { qkor("삼성전자"),qkor("코스피"),
		qkor("코스닥"),qkor("삼성바이오"),
		qkor("KODEX 레버리지"),qkor("KODEX 인버스"),qkor("셀트리온") };
	QString saveName[BOX_SZ];
	int nameFlag[BOX_SZ];
	stockInfo info[10];

	addWidget();
	~addWidget();
	QString qkor(const char *str);
	void setUpWidget()
	{


		std::locale::global(std::locale("kor"));
		window->resize(292, 449);
		window->setWindowTitle(qkor("추가"));
		window->setStyleSheet(QStringLiteral("QTabBar::tab { width: 134px; }"));


		lineEdit->setPlaceholderText("Search for stock name.");
		label->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

		/* first Tab */
		scrollArea->setWidgetResizable(true);
		scrollAreaWidgetContents->setGeometry(QRect(0, 0, 248, 287));
		scrollArea->setWidget(scrollAreaWidgetContents);

		gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

		/* second Tab */
		scrollArea_2->setWidgetResizable(true);
		scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 248, 287));
		scrollArea_2->setWidget(scrollAreaWidgetContents_2);


		gridLayout_2->addWidget(scrollArea_2, 0, 0, 1, 1);

		tabWidget->addTab(viewTab, QString());
		tabWidget->addTab(listTab, QString());

		pushButton->setText(qkor("완료"));
		connect(pushButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));

		verticalLayout->addWidget(lineEdit);
		verticalLayout->addWidget(label);
		verticalLayout->addWidget(tabWidget);
		verticalLayout->addWidget(pushButton);

		tabWidget->setTabText(tabWidget->indexOf(viewTab), QString::fromLocal8Bit("최근 조회"));
		tabWidget->setTabText(tabWidget->indexOf(listTab), QString::fromLocal8Bit("주식 목록"));


		window->show();

	}
	QString* returnName(QString *_name);

	public slots:
	void checkBox_clicked(int i);
	void okButton_clicked();

public:

	int flag = 1;

};

#endif // ADDWIDGET_H
