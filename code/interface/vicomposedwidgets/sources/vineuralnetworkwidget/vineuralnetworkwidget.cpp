#include <vineuralnetworkwidget.h>

#include <vineuralnetworkfactory.h>
#include <visigmoidactivationfunction.h>

#define DEFAULT_NEURON_RADIUS 25

ViNeuralNetworkWidget::ViNeuralNetworkWidget(ViWidget *parent)
	: ViWidget(parent)
{
	mNeuralNetwork = NULL;
	mNeuronSize = DEFAULT_NEURON_RADIUS;

	ViNeuralNetworkFactory mFactory;
	mFactory.setActivationFunction(new ViSigmoidActivationFunction());
	mFactory.addLayer(5);
	mFactory.addLayer(3, 1);
	mFactory.addLayer(4);
	setNeuralNetwork(mFactory.create());

	setStyleSheet("background-color:red;");
}
		
void ViNeuralNetworkWidget::setNeuralNetwork(ViNeuralNetwork *neuralNetwork)
{
	mNeuralNetwork = neuralNetwork;
	repaint();
}

ViNeuralNetwork* ViNeuralNetworkWidget::neuralNetwork()
{
	return mNeuralNetwork;
}

void ViNeuralNetworkWidget::setNeuronSize(int radius)
{
	mNeuronSize = radius;
}

int ViNeuralNetworkWidget::neuronSize()
{
	return mNeuronSize;
}

void ViNeuralNetworkWidget::resizeEvent(QResizeEvent *event)
{
	refreshWidgets();
}

void ViNeuralNetworkWidget::refreshWidgets()
{
	if(mNeuralNetwork != NULL)
	{
		mNeuronWidgets.clear();
		int xOffset = (height() - (((mNeuralNetwork->size() * 3) - 1) * mNeuronSize)) / 2;
		for(int i = 0; i < mNeuralNetwork->size(); ++i)
		{
			int yOffset = (height() - (((mNeuralNetwork->at(i)->size() * 3) - 1) * mNeuronSize)) / 2;
			for(int j = 0; j < mNeuralNetwork->at(i)->size(); ++j)
			{
				mNeuronWidgets.append(new ViNeuronWidget(xOffset + (i * 3 * mNeuronSize), yOffset + (j * 3 * mNeuronSize), mNeuronSize, this));
			}
		}
	}
}
