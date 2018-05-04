/********************************************************************************
** Form generated from reading UI file 'Stock.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCK_H
#define UI_STOCK_H

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

QT_BEGIN_NAMESPACE

class Ui_StockClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *StockClass)
    {
        if (StockClass->objectName().isEmpty())
            StockClass->setObjectName(QStringLiteral("StockClass"));
        StockClass->resize(600, 400);
        centralWidget = new QWidget(StockClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        StockClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(StockClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        StockClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(StockClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        StockClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(StockClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        StockClass->setStatusBar(statusBar);

        retranslateUi(StockClass);

        QMetaObject::connectSlotsByName(StockClass);
    } // setupUi

    void retranslateUi(QMainWindow *StockClass)
    {
        StockClass->setWindowTitle(QApplication::translate("StockClass", "Stock", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StockClass: public Ui_StockClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCK_H
