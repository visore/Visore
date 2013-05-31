#ifndef VINEURALSELECTORWIDGET_H
#define VINEURALSELECTORWIDGET_H

#include <viwidget.h>

namespace Ui
{
    class ViNeuralSelectorWidget;
}

class ViNeuralSelectorWidget : public ViWidget
{

	Q_OBJECT

	private slots:

		void process();

		void addHiddenLayer();
		void removeHiddenLayer();

		void addErrorFunction();
		void removeErrorFunction();

		void toggleTargetProvider();

	public:

		ViNeuralSelectorWidget(QWidget *parent = 0);
		~ViNeuralSelectorWidget();

	private:

		Ui::ViNeuralSelectorWidget *mUi;

};

#endif
