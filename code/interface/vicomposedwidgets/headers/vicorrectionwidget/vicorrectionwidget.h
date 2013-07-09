#ifndef VICORRECTIONINGWIDGET_H
#define VICORRECTIONINGWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViCorrectionWidget;
}

class ViCorrectionWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void selectCorrector();
		void selectMode();

	public:

		ViCorrectionWidget(QWidget *parent = 0);
		~ViCorrectionWidget();

	private:

		Ui::ViCorrectionWidget *mUi;

};

#endif
