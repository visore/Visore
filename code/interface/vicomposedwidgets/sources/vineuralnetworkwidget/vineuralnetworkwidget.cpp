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
	mFactory.addLayer(1);
	setNeuralNetwork( mFactory.create());
}

ViNeuralNetworkWidget::~ViNeuralNetworkWidget()
{
	for(int i = 0; i < mNeuronWidgets.size(); ++i)
	{
		viDeleteAll(mNeuronWidgets[i]);
	}
	mNeuronWidgets.clear();
	viDeleteAll(mBiasWidgets);
	viDeleteAll(mSynapseWidgets);
	viDeleteAll(mLineWidgets);
}
		
void ViNeuralNetworkWidget::setNeuralNetwork(ViNeuralNetwork *neuralNetwork)
{
	mNeuralNetwork = neuralNetwork;
	if(mNeuralNetwork != NULL)
	{
		mNeuronWidgets.clear();

		//Determine if there is a bias
		mHasBias = false;
		for(int i = 0; i < mNeuralNetwork->size(); ++i)
		{
			if(mNeuralNetwork->at(i)->hasBias())
			{
				mHasBias = true;
				break;
			}
		}

		//Add neurons
		for(int i = 0; i < mNeuralNetwork->size(); ++i)
		{
			QList<ViNeuronWidget*> layer;
			for(int j = 0; j < mNeuralNetwork->at(i)->size(); ++j)
			{
				layer.append(new ViNeuronWidget(xNeuronOffset(i), yNeuronOffset(i, j, mHasBias), mNeuronSize, mNeuralNetwork->at(i)->at(j)));
			}

			//Add bias
			if(mNeuralNetwork->at(i)->hasBias())
			{
				mBiasWidgets.append(new ViBiasWidget(xBiasOffset(i), yBiasOffset(), mNeuronSize, mNeuralNetwork->at(i)->bias()));
			}
			else
			{
				mBiasWidgets.append(NULL);
			}
			mNeuronWidgets.append(layer);
		}

		//Add synapses
		for(int i = 0; i < mNeuralNetwork->size() - 1; ++i)
		{
			ViNeuralLayer *inputLayer = mNeuralNetwork->at(i);
			for(int j = 0; j < inputLayer->size(); ++j)
			{
				ViNeuron *neuron = inputLayer->at(j);
				for(int n = 0; n < neuron->outputSize(); ++n)
				{
					ViNeuronWidget *inputWidget = neuronWidget(neuron);
					ViNeuronWidget *outputWidget = neuronWidget(neuron->outputAt(n)->output());
					if(inputWidget != NULL && outputWidget != NULL)
					{
						mSynapseWidgets.append(new ViSynapseWidget(inputWidget, outputWidget, neuron->outputAt(n), this));
					}
				}
			}

			//Add bias synapses
			if(inputLayer->hasBias())
			{
				ViNeuron *neuron = inputLayer->bias();
				for(int n = 0; n < neuron->outputSize(); ++n)
				{
					ViNeuronWidget *inputWidget = neuronWidget(neuron);
					ViNeuronWidget *outputWidget = neuronWidget(neuron->outputAt(n)->output());
					if(inputWidget != NULL && outputWidget != NULL)
					{
						mSynapseWidgets.append(new ViSynapseWidget(inputWidget, outputWidget, neuron->outputAt(n), this));
					}
				}
			}
		}

		//Add input lines
		for(int i = 0; i < mNeuronWidgets[0].size(); ++i)
		{
			mLineWidgets.append(new ViNeuronLineWidget(mNeuronWidgets[0][i], this));
		}

		//Add output lines
		for(int i = 0; i < mNeuronWidgets[mNeuronWidgets.size() - 1].size(); ++i)
		{
			mLineWidgets.append(new ViNeuronLineWidget(mNeuronWidgets[mNeuronWidgets.size() - 1][i], this));
		}

		//Make sure the neurons are drawn last
		for(int i = 0; i < mNeuronWidgets.size(); ++i)
		{
			for(int j = 0; j < mNeuronWidgets[i].size(); ++j)
			{
				mNeuronWidgets[i][j]->setParent(this);
			}
		}
		for(int i = 0; i < mBiasWidgets.size(); ++i)
		{
			if(mBiasWidgets[i] != NULL)
			{
				mBiasWidgets[i]->setParent(this);
			}
		}
	}
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

ViNeuronWidget* ViNeuralNetworkWidget::neuronWidget(ViNeuron *neuron)
{
	for(int i = 0; i < mNeuronWidgets.size(); ++i)
	{
		for(int j = 0; j < mNeuronWidgets[i].size(); ++j)
		{
			if(mNeuronWidgets[i][j]->neuron() == neuron)
			{
				return mNeuronWidgets[i][j];
			}
		}
	}
	for(int i = 0; i < mBiasWidgets.size(); ++i)
	{
		if(mBiasWidgets[i] != NULL && mBiasWidgets[i]->neuron() == neuron)
		{
			return mBiasWidgets[i];
		}
	}
	return NULL;
}

void ViNeuralNetworkWidget::resizeEvent(QResizeEvent *event)
{
	refreshWidgets();
}

int ViNeuralNetworkWidget::xNeuronOffset(int layer)
{
	if(mNeuralNetwork == NULL)
	{
		return 0;
	}
	int offset = (width() - (mNeuralNetwork->size() * mNeuronSize * 2)) / (1 + mNeuralNetwork->size());
	return (layer * mNeuronSize * 2) + ((layer + 1) * offset);
}

int ViNeuralNetworkWidget::yNeuronOffset(int layer, int neuron, bool bias)
{
	if(mNeuralNetwork == NULL)
	{
		return 0;
	}
	int count = mNeuralNetwork->at(layer)->size();
	int newHeight = height();
	if(bias)
	{
		newHeight -= mNeuronSize * 2;
	}
	int offset = (newHeight - (count * mNeuronSize * 2)) / (1 + count);
	return (neuron * mNeuronSize * 2) + ((neuron + 1) * offset);
}

int ViNeuralNetworkWidget::xBiasOffset(int layer)
{
	return (xNeuronOffset(layer) + xNeuronOffset(layer - 1)) / 2;
}

int ViNeuralNetworkWidget::yBiasOffset()
{
	return height() - (mNeuronSize * 3);
}

void ViNeuralNetworkWidget::refreshWidgets()
{
	if(mNeuralNetwork != NULL)
	{
		for(int i = 0; i < mNeuronWidgets.size(); ++i)
		{
			for(int j = 0; j < mNeuronWidgets[i].size(); ++j)
			{
				mNeuronWidgets[i][j]->move(xNeuronOffset(i), yNeuronOffset(i, j, mHasBias));
			}
		}
		for(int i = 0; i < mBiasWidgets.size(); ++i)
		{
			if(mBiasWidgets[i] != NULL)
			{
				mBiasWidgets[i]->move(xBiasOffset(i), yBiasOffset());
			}
		}
	}
}
