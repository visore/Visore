#ifndef VINEURALNETWORKWIDGET_H
#define VINEURALNETWORKWIDGET_H

#include <vineuronwidget.h>
#include <vineuralnetwork.h>
#include <QList>

class ViNeuralNetworkWidget : public ViWidget
{

	Q_OBJECT

	public:

		ViNeuralNetworkWidget(ViWidget *parent = NULL);
		
		void setNeuralNetwork(ViNeuralNetwork *neuralNetwork);
		ViNeuralNetwork* neuralNetwork();

		void setNeuronSize(int radius);
		int neuronSize();

	protected:

		void resizeEvent(QResizeEvent *event);

		void refreshWidgets();

	private:

		ViNeuralNetwork *mNeuralNetwork;
		QList<ViNeuronWidget*> mNeuronWidgets;
		int mNeuronSize;

};

#endif
