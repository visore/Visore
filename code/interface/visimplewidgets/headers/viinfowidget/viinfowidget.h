#ifndef VIINFOWIDGET_H
#define VIINFOWIDGET_H

#include <QVBoxLayout>
#include "viwidget.h"
#include "viinfolabel.h"

class ViInfoWidget : public ViWidget
{

	Q_OBJECT

	public slots:

		void addEntry(QString description, QString text);
		void addEntry(QString text);

		void updateEntry(int index, QString description, QString text);
		void updateEntry(int index, QString text);

	public:

		ViInfoWidget(QWidget *parent = 0);
		virtual ~ViInfoWidget();

	protected:

		void update();

	protected:

		QVBoxLayout *mLayout;
		QList<ViInfoLabel*> mItems;
		
};

#endif
