#ifndef VIPROJECTWIDGET_H
#define VIPROJECTWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"

namespace Ui
{
    class ViProjectWidget;
}

class ViProjectWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void start();

	public:

		ViProjectWidget(QWidget *parent = 0);
		~ViProjectWidget();

	private:

		Ui::ViProjectWidget *mUi;

};

#endif
