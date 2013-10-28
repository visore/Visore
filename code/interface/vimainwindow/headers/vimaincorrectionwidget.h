#ifndef VIMAINCORRECTIONWIDGET_H
#define VIMAINCORRECTIONWIDGET_H

#include <viwidget.h>

namespace Ui
{
	class ViMainCorrectionWidget;
}

class ViMainCorrectionWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void showContainer();

		void changeCorrector();
		void changeMode();

		void correct();
        void showCorrelation();

	public:

		ViMainCorrectionWidget(QWidget *parent = 0);
		~ViMainCorrectionWidget();

		void clear();

	private:

		Ui::ViMainCorrectionWidget *mUi;

};

#endif
