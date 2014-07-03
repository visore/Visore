#include <vineuralpredictor.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 32

ViNeuralPredictor::ViNeuralPredictor()
	: ViPredictor()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);

	addParameterName("Window Size");

	addParameterName("l1");
	addParameterName("l2");
	addParameterName("l3");
	addParameterName("l4");
	addParameterName("l5");
	l1=l2=l3=l4=l5=0;
}

ViNeuralPredictor::ViNeuralPredictor(const ViNeuralPredictor &other)
	: ViPredictor(other)
{
	for(int i = 0; i < other.mNetworks.size(); ++i) mNetworks.append(new ViFann(*other.mNetworks[i]));
}

ViNeuralPredictor::~ViNeuralPredictor()
{
	clear();
}

void ViNeuralPredictor::setWindowSize(const int &size)
{
	mInputs = size;
	ViPredictor::setWindowSize(mInputs + 1);
	setOffset(mInputs);
}

void ViNeuralPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);

	else if(number==1)l1=value;
	else if(number==2)l2=value;
	else if(number==3)l3=value;
	else if(number==4)l4=value;
	else if(number==5)l5=value;

	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

void ViNeuralPredictor::clear()
{
	for(int i = 0; i < mNetworks.size(); ++i) delete mNetworks[i];
	mNetworks.clear();
}

void ViNeuralPredictor::initialize(const int &channelCount, const int &predictionCount)
{
	int i;
	clear();
	for(i = 0; i < channelCount; ++i) mNetworks.append(new ViFann());

	ViPredictor::setWindowSize(mInputs + predictionCount);
	setOffset(mInputs);

	QList<int> neurons;
	neurons.append(mInputs);
	if(l1>0)
	{
		neurons.append(l1);
		if(l2>0)
		{
			neurons.append(l2);
			if(l3>0)
			{
				neurons.append(l3);
				if(l4>0)
				{
					neurons.append(l4);
					if(l5>0)
					{
						neurons.append(l5);
					}
				}
			}
		}
	}

	//neurons.append(16);
	//neurons.append(8);
	//neurons.append(mInputs / 2);
	//neurons.append(mInputs / 3);
	//neurons.append(1);
	neurons.append(predictionCount);
	/*neurons.append(16);
	neurons.append(8);
	neurons.append(1);*/

	for(i = 0; i < channelCount; ++i)
	{
		mNetworks[i]->setStructure(ViFann::Standard, neurons);
		mNetworks[i]->setActivation(ViFann::Elliot);
		//mNetworks[i]->setWeights(ViFann::WidrowNguyen);
		mNetworks[i]->setWeights(ViFann::Random);

		//mNetworks[i]->setLearningRate(0.9);
		mNetworks[i]->setLearningRate(0.2);
		mNetworks[i]->setLearningMomentum(0.1);
	}

	//setPredictionIndex(18);
}

bool ViNeuralPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error, const int &channel)
{
	/*mNetworks[channel]->train(samples, samples[size - 1]);
	mNetworks[channel]->run(samples, predictedSamples, predictionCount);*/

	mNetworks[channel]->run(samples, predictedSamples);
	mNetworks[channel]->train(samples, samples + mInputs);
}
