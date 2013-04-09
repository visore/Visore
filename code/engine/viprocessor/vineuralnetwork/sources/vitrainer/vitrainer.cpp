#include <vitrainer.h>
#include <vilogger.h>
#include <vierrorfunctionmanager.h>

#define DEFAULT_LEARNING_RATE 0.1

ViTrainer::ViTrainer()
{
	mNetwork = NULL;
	mErrorFunction = NULL;
	clear();
}

ViTrainer::ViTrainer(const ViTrainer &other)
{
	mNetwork = other.mNetwork;

	mLearningRate = other.mLearningRate;
	mIterationLimit = other.mIterationLimit;
	mErrorLimit = other.mErrorLimit;
	mCurrentIteration = other.mCurrentIteration;
	mCurrentError = other.mCurrentError;
}

ViTrainer::~ViTrainer()
{
}

void ViTrainer::clear()
{
	setNetwork(NULL);
	setLearningRate(DEFAULT_LEARNING_RATE);
	setIterationLimit(-1); //Unlimited
	setErrorLimit(0);
	mCurrentIteration = -1;
	mCurrentError = 1;

	if(mErrorFunction != NULL)
	{
		delete mErrorFunction;
		mErrorFunction = NULL;
	}
}

ViNeuralNetwork* ViTrainer::network()
{
	return mNetwork;
}

void ViTrainer::setNetwork(ViNeuralNetwork *network)
{
	mNetwork = network;
}

int ViTrainer::iterationLimit()
{
	return mIterationLimit;
}

void ViTrainer::setIterationLimit(int limit)
{
	mIterationLimit = limit;
}

qreal ViTrainer::errorLimit()
{
	return mErrorLimit;
}

void ViTrainer::setErrorLimit(qreal limit)
{
	mErrorLimit = limit;
}

int ViTrainer::currentIteration()
{
	return mCurrentIteration;
}

qreal ViTrainer::currentError()
{
	return mCurrentError;
}

void ViTrainer::setLearningRate(qreal learningRate)
{
	mLearningRate = learningRate;
}

qreal ViTrainer::learningRate()
{
	return mLearningRate;
}

void ViTrainer::setErrorFunction(ViErrorFunction *function)
{
	if(mErrorFunction != NULL)
	{
		delete mErrorFunction;
	}
	mErrorFunction = function;
}

ViErrorFunction* ViTrainer::errorFunction()
{
	return mErrorFunction;
}

void ViTrainer::calculateError()
{
	//mErrorFunction->setRealValue(mNetwork->outputs());
	//Set target value here
	//mCurrentError = mErrorFunction->calculate();
}

void ViTrainer::train()
{
	if(mIterationLimit < 0)
	{
		mCurrentIteration = 0;
		while(mCurrentError > mErrorLimit)
		{
			mNetwork->run();
			calculateError();
			trainNetwork();
			++mCurrentIteration;
		}
	}
	else
	{
		for(mCurrentIteration = 0; mCurrentIteration < mIterationLimit; ++mCurrentIteration)
		{
			mNetwork->run();
			calculateError();
			if(mCurrentError <= mErrorLimit)
			{
				break;
			}
			trainNetwork();
		}
	}
}

void ViTrainer::train(ViNeuralNetwork *network)
{
	setNetwork(network);
	train();
}

void ViTrainer::train(ViNeuralNetwork *network, int iterationLimit, qreal errorLimit)
{
	setIterationLimit(iterationLimit);
	setErrorLimit(errorLimit);
	train(network);
}

void ViTrainer::trainSingle()
{
	setIterationLimit(1);
	train();
}

void ViTrainer::trainSingle(ViNeuralNetwork *network)
{
	setNetwork(network);
	trainSingle();
}

ViElement ViTrainer::exportData()
{
	ViElement element("Trainer");
	element.addChild("Name", name());
	element.addChild("IterationLimit", iterationLimit());
	element.addChild("ErrorLimit", errorLimit());
	element.addChild("LearningRate", learningRate());

	if(errorFunction() == NULL)
	{
		element.addChild(errorFunction()->exportData());
	}
	else
	{
		LOG("There is no error function to export.", QtCriticalMsg);
	}

	return element;
}

bool ViTrainer::importData(ViElement element)
{
	if(element.name() != "Trainer")
	{
		element = element.child("Trainer");
		if(element.name() != "Trainer")
		{
			return false;
		}
	}

	ViElement theName = element.child("name");
	if(theName.isNull())
	{
		return false;
	}
	if(theName.toString() != name())
	{
		return false;
	}

	ViElement limit = element.child("IterationLimit");
	if(limit.isNull())
	{
		LOG("The iteration limit could not be imported", QtCriticalMsg);
	}
	else
	{
		setIterationLimit(limit.toInt());
	}

	limit = element.child("ErrorLimit");
	if(limit.isNull())
	{
		LOG("The error limit could not be imported", QtCriticalMsg);
	}
	else
	{
		setErrorLimit(limit.toReal());
	}

	limit = element.child("LearningRate");
	if(limit.isNull())
	{
		LOG("The learning could not be imported", QtCriticalMsg);
	}
	else
	{
		setLearningRate(limit.toReal());
	}

	setErrorFunction(ViErrorFunctionManager::create(element));
	if(errorFunction() == NULL)
	{
		return false;
	}

	return true;
}
