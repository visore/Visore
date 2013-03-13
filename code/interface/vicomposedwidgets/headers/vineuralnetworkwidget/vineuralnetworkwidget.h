#ifndef VINEURALNETWORKWIDGET_H
#define VINEURALNETWORKWIDGET_H

#include <visynapsewidget.h>
#include <vineuronlinewidget.h>
#include <vibiaswidget.h>
#include <vineuralnetwork.h>
#include <QList>

class ViNeuralNetworkWidget : public ViWidget
{

	Q_OBJECT

	public:

		ViNeuralNetworkWidget(ViWidget *parent = NULL);
		~ViNeuralNetworkWidget();
		
		void setNeuralNetwork(ViNeuralNetwork *neuralNetwork);
		ViNeuralNetwork* neuralNetwork();

		void setNeuronSize(int radius);
		int neuronSize();

	protected:

		ViNeuronWidget* neuronWidget(ViNeuron *neuron);

		void resizeEvent(QResizeEvent *event);

		int xNeuronOffset(int layer);
		int yNeuronOffset(int layer, int neuron, bool bias = false);
		int xBiasOffset(int layer);
		int yBiasOffset();
		void refreshWidgets();

	private:

		ViNeuralNetwork *mNeuralNetwork;
		QList<QList<ViNeuronWidget*> > mNeuronWidgets;
		QList<ViBiasWidget*> mBiasWidgets;
		QList<ViSynapseWidget*> mSynapseWidgets;
		QList<ViNeuronLineWidget*> mLineWidgets;
		int mNeuronSize;
		bool mHasBias;

};

#endif
