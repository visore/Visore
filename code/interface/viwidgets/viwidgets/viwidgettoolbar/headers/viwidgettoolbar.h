#ifndef VIWIDGETTOOLBAR_H
#define VIWIDGETTOOLBAR_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
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
		ViWidgetToolbar(ViWidgetToolbar::ViWidgetToolbarAlign align, ViAudioEngine *engine, QWidget *parent = 0);
		~ViWidgetToolbar();
		void refresh();

		//Specify the widget and the function to call on the widget. Use SLOT(...) as last parameter
		void addButton(QString text, QIcon icon, ViWidget *widget, const char *function);

	private:
		QBoxLayout *mLayout;
		QWidget *mCentralWidget;
		ViWidgetToolbar::ViWidgetToolbarAlign mAlign;
		QList<QToolButton*> mButtons;
};

#endif
