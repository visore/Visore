#ifndef VIOUTPUTCONTROLWIDGET_H
#define VIOUTPUTCONTROLWIDGET_H

#include <QFileDialog>
#include "vithememanager.h"
#include "viwidget.h"

namespace Ui
{
    class ViOutputControlWidget;
}

class ViOutputControlWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void selectFileOutput();
		void save();

	public:
		ViOutputControlWidget(QWidget *parent = 0);
		~ViOutputControlWidget();

	private:
		void populate();
		void setDefaults();

	private:
		Ui::ViOutputControlWidget *mUi;
};

#endif
