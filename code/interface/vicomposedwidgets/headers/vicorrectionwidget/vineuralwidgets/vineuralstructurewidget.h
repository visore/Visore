#ifndef VINEURALSTRUCTUREWIDGET_H
#define VINEURALSTRUCTUREWIDGET_H

#include <viwidget.h>
#include <vineuralnetwork.h>
#include <vicorrectionmode.h>

namespace Ui
{
	class ViNeuralStructureWidget;
}

class ViNeuralStructureWidget : public ViWidget, public ViCorrectionMode
{

	Q_OBJECT

	private slots:

		void addHiddenLayer();
		void addHiddenLayer(int neurons, double bias = 0);
		void removeHiddenLayer();

	public:

		ViNeuralStructureWidget(QWidget *parent = 0);
		~ViNeuralStructureWidget();

		void setMode(ViCorrectionMode::Mode mode);

		ViNeuralNetwork* network();

	private:

		Ui::ViNeuralStructureWidget *mUi;

};

#endif
