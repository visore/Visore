#ifndef VINEURALWEIGHTINITIALIZERWIDGET_H
#define VINEURALWEIGHTINITIALIZERWIDGET_H

#include <viwidget.h>
#include <vicorrectionmode.h>
#include <viweightinitializer.h>

namespace Ui
{
	class ViNeuralWeightInitializerWidget;
}

class ViNeuralWeightInitializerWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	public:

		ViNeuralWeightInitializerWidget(QWidget *parent = 0);
		~ViNeuralWeightInitializerWidget();

		void setMode(ViCorrectionMode::Mode mode);

		ViWeightInitializer* weightInitializer();

	private:

		Ui::ViNeuralWeightInitializerWidget *mUi;
		QString mDefaultInitializer;

};

#endif
