#ifndef VIWIDGETTOOLBARBUTTON_H
#define VIWIDGETTOOLBARBUTTON_H

#include <QToolButton>
#include "vithememanager.h"

class ViWidgetToolbarButton : public QToolButton
{
	Q_OBJECT

	signals:
		void clickedWithId(qint8 id);

	private slots:
		void clickWithId();

	public:
		ViWidgetToolbarButton(qint8 id, QWidget *parent = 0);
		~ViWidgetToolbarButton();

	private:
		qint8 mId;
};

#endif
