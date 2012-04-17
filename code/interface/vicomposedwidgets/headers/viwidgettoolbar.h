#ifndef VIWIDGETTOOLBAR_H
#define VIWIDGETTOOLBAR_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "viwidget.h"
#include "viwidget.h"
#include "vithememanager.h"

class ViWidgetToolbar : public ViWidget
{
	Q_OBJECT

	signals:
		void buttonClicked(qint8 buttonId);

	public:
		enum ViWidgetToolbarAlign
		{
			Top = 0,
			Bottom = 1,
			Right = 2,
			Left = 3
		};
		ViWidgetToolbar(Qt::Alignment align, QWidget *parent = 0);
		~ViWidgetToolbar();
		void refresh();
		void addWidget(QWidget *widget);
		void show();
		void hide();
		void setVisible(bool visible);

	private:
		QBoxLayout *mLayout;
		QWidget *mCentralWidget;
		Qt::Alignment mAlign;
};

#endif
