#ifndef VIMAINCORRECTIONWIDGET_H
#define VIMAINCORRECTIONWIDGET_HH

#include <viwidget.h>

namespace Ui
{
	class ViMainCorrectionWidget;
}

class ViMainCorrectionWidget : public ViWidget
{
	Q_OBJECT

	private slots:

		void changeCorrector();
		void changeMode();

		void correct();
		void showCorrector();
		void hideCorrector();
        void showCorrelation();

	public:

		ViMainCorrectionWidget(QWidget *parent = 0);
		~ViMainCorrectionWidget();

		void clear();

    protected:

        void hideEvent(QHideEvent *event);

	private:

		Ui::ViMainCorrectionWidget *mUi;

};

#endif
