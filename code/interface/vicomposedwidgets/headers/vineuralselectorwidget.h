#ifndef VINEURALSELECTORWIDGET_H
#define VINEURALSELECTORWIDGET_H

#include <viwidget.h>
#include <vineuralcorrector.h>

namespace Ui
{
    class ViNeuralSelectorWidget;
}

class ViNeuralSelectorWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void addHiddenLayer();
		void addHiddenLayer(int neurons, double bias = 0);
		void removeHiddenLayer();

		void addErrorFunction();
		void removeErrorFunction();

		void toggleTargetProvider();

		void changeSettings(int index);

	public:

		ViNeuralSelectorWidget(QWidget *parent = 0);
		~ViNeuralSelectorWidget();

		ViNeuralCorrector* corrector();

	private:

		Ui::ViNeuralSelectorWidget *mUi;

};

#endif
