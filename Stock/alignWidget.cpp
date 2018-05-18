#include "alignWidget.h"

alignWidget::alignWidget()
{
	QSignalMapper *signalMapper = new QSignalMapper(this);
	for (int i = 0; i < 5; i++)
	{
		btn[i] = new QPushButton(window);
		btn[i]->setGeometry(QRect(20, 10 + i * 30, 151, 31));
		btn[i]->setText(btn_name[i]);
		QObject::connect(btn[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
		signalMapper->setMapping(btn[i], i);
		QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(button_clicked(int)));

	}
}

alignWidget::~alignWidget()
{
}

QString alignWidget::qkor(const char *str)
{
	return QString::fromLocal8Bit(str);
}

void alignWidget::button_clicked(int i)
{
	m = i;
	window->close();
		
}
