#ifndef VINEURALTARGETPROVIDERWIDGET_H
#define VINEURALTARGETPROVIDERWIDGET_H

#include <viwidget.h>
#include <vicorrectionmode.h>
#include <vitargetprovider.h>

namespace Ui
{
	class ViNeuralTargetProviderWidget;
}

class ViNeuralTargetProviderWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	private slots:

		void toggleProvider();

	public:

		ViNeuralTargetProviderWidget(QWidget *parent = 0);
		~ViNeuralTargetProviderWidget();

		void setMode(ViCorrectionMode::Mode mode);

		ViTargetProvider* targetProvider();

	private:

		Ui::ViNeuralTargetProviderWidget *mUi;
		QString mDefaultProvider;
		QString mDefaultInterpolator;

};

#endif
