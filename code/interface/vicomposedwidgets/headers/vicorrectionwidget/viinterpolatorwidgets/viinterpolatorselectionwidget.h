#ifndef VIINTERPOLATORSELECTIONWIDGET_H
#define VIINTERPOLATORSELECTIONWIDGET_H

#include <viwidget.h>
#include <vicorrectionmode.h>
#include <viinterpolator.h>

namespace Ui
{
	class ViInterpolatorSelectionWidget;
}

class ViInterpolatorSelectionWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	public:

		ViInterpolatorSelectionWidget(QWidget *parent = 0);
		~ViInterpolatorSelectionWidget();

		void setMode(ViCorrectionMode::Mode mode);

		ViInterpolator* interpolator();

	private:

		Ui::ViInterpolatorSelectionWidget *mUi;
		QString mDefaultInterpolator;

};

#endif
