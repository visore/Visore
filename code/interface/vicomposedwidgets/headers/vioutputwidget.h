#ifndef VIOUTPUTWIDGET_H
#define VIOUTPUTWIDGET_H

#include <QFileDialog>
#include <QGridLayout>
#include "vithememanager.h"
#include "viwidget.h"
#include "vioutputcontrolwidget.h"

namespace Ui
{
    class ViOutputWidget;
}

class ViOutputWidget : public ViWidget
{
	Q_OBJECT

	private slots:
		void selectFileOutput();

	public:
		ViOutputWidget(QWidget *parent = 0);
		~ViOutputWidget();

	private:
		Ui::ViOutputWidget *mUi;
		QDialog *mDialog;
		QGridLayout *mLayout;
};

#endif
