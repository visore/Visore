#ifndef VICORRELATIONINGWIDGET_H
#define VICORRELATIONINGWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViCorrelationWidget;
}

class ViCorrelationWidget : public ViWidget
{
	Q_OBJECT

	public slots:

		void recalculate();
		void update();

	public:

		ViCorrelationWidget(QWidget *parent = 0);
		~ViCorrelationWidget();

	protected:

		void showEvent(QShowEvent *event);
		void adjustTable();

	private:

		Ui::ViCorrelationWidget *mUi;
		bool mWasInitialized;

};

#endif
