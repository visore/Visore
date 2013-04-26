#ifndef VIPROJECTCORRECTIONWIDGET_H
#define VIPROJECTCORRECTIONWIDGET_H

#include "viwidget.h"
#include "vithememanager.h"
#include "vibutton.h"

namespace Ui
{
    class ViProjectCorrectionWidget;
}

class ViProjectCorrectionWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void correct();

	public:

		ViProjectCorrectionWidget(QWidget *parent = 0);
		~ViProjectCorrectionWidget();

	private:

		Ui::ViProjectCorrectionWidget *mUi;

};

#endif
