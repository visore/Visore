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
		void removeHiddenLayer();

		void addErrorFunction();
		void removeErrorFunction();

		void toggleTargetProvider();

	public:

		ViNeuralSelectorWidget(QWidget *parent = 0);
		~ViNeuralSelectorWidget();

		ViNeuralCorrector* corrector();

	private:

		Ui::ViNeuralSelectorWidget *mUi;

};

#endif
