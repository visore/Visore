#ifndef VINEURALACTIVATIONFUNCTIONWIDGET_H
#define VINEURALACTIVATIONFUNCTIONWIDGET_H

#include <viwidget.h>
#include <vicorrectionmode.h>
#include <viactivationfunction.h>

namespace Ui
{
	class ViNeuralActivationFunctionWidget;
}

class ViNeuralActivationFunctionWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	public:

		ViNeuralActivationFunctionWidget(QWidget *parent = 0);
		~ViNeuralActivationFunctionWidget();

		void setMode(ViCorrectionMode::Mode mode);

		ViActivationFunction* activationFunction();

	private:

		Ui::ViNeuralActivationFunctionWidget *mUi;
		QString mDefaultFunction;

};

#endif
