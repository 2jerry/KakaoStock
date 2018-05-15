/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QSignalMapper>


QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
	QMenuBar * menuBar;
	QToolBar *mainToolBar;
	QWidget *centralWidget;
	QStatusBar *statusBar;
	QPushButton *btn[3];

	void setupUi(QMainWindow *MainWindow)
	{
		if (MainWindow->objectName().isEmpty())
			MainWindow->setObjectName(QStringLiteral("MainWindow"));
		MainWindow->resize(316, 408);
		 //MainWindow->setStyleSheet("background:rgb(255,255,255)");
		menuBar = new QMenuBar(MainWindow);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		MainWindow->setMenuBar(menuBar);

		QFont font;
		font.setFamily(QString::fromUtf8("\353\217\213\354\233\200"));
		font.setPointSize(10.5);

		centralWidget = new QWidget(MainWindow);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		MainWindow->setCentralWidget(centralWidget);
		statusBar = new QStatusBar(MainWindow);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		MainWindow->setStatusBar(statusBar);

		for (int i = 0; i<3; i++)
		{
			btn[i] = new QPushButton(MainWindow);
			btn[i]->setGeometry(QRect(i * 105, 0, 106, 31));
			btn[i]->setFont(font);
			btn[i]->setStyleSheet("background:rgba(240,240,240,0)"); // ¹öÆ°
			
		}
		retranslateUi(MainWindow);

		QMetaObject::connectSlotsByName(MainWindow);
	} // setupUi

	void retranslateUi(QMainWindow *MainWindow)
	{
		MainWindow->setWindowTitle(QApplication::translate("MainWindow", "StockProgram", nullptr));
		btn[0]->setText(QApplication::translate("MainWindow", "\355\216\270\354\247\221", nullptr));
		btn[1]->setText(QApplication::translate("MainWindow", "\354\242\205\353\252\251\354\266\224\352\260\200", nullptr));
		btn[2]->setText(QApplication::translate("MainWindow", "\354\240\225\353\240\254", nullptr));
	} // retranslateUi

};

namespace Ui {
	class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
