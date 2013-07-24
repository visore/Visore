#include <vitrainer.h>
#include <vilogger.h>
#include <vierrorfunctionmanager.h>

#define DEFAULT_LEARNING_RATE 0.1

ViTrainer::ViTrainer()
{
	mNetwork = NULL;
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
	for(int i = 0; i < other.mErrorFunctions.size(); ++i)
	{
		addErrorFunction(other.mErrorFunctions[i]->clone());
	}
}

ViTrainer::~ViTrainer()
{
}

void ViTrainer::clear()
{
	setNetwork(NULL);
	setLearningRate(DEFAULT_LEARNING_RATE);
	setIterationLimit(0); //Unlimited
	setErrorLimit(0);
	mCurrentIteration = -1;
	mCurrentError = 1;
	viDeleteAll(mErrorFunctions);
}

ViNeuralNetwork* ViTrainer::network()
{
	return mNetwork;
}

void ViTrainer::setNetwork(ViNeuralNetwork *network)
{
	mNetwork = network;
}

const ViRealList& ViTrainer::targetValues()
{
	return mTargetValues;
}

void ViTrainer::setTargetValues(ViRealList values)
{
	mTargetValues = values;
}

void ViTrainer::checkTargetValues()
{
	if(mNetwork != NULL)
	{
		if(mTargetValues.size() != mNetwork->outputCount())
		{
			LOG("The number of target values does not correspond with the number of neural network outputs.", QtCriticalMsg);
		}
	}
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

void ViTrainer::addErrorFunction(ViErrorFunction *function)
{
	if(function != NULL)
	{
		mErrorFunctions.append(function);
	}
}

void ViTrainer::addErrorFunctions(QList<ViErrorFunction*> functions)
{
	for(int i = 0; i < functions.size(); ++i)
	{
		addErrorFunction(functions[i]);
	}
}

void ViTrainer::calculateError()
{
	//QString message = "";
	qreal error;
	mCurrentError = 0;
	for(int i = 0; i < mNetwork->outputCount(); ++i)
	{
		mCurrentError += qAbs(mNetwork->output(i) - mTargetValues[i]);
	}
	mCurrentError /= mNetwork->outputCount();
	//message += "Current error: " + QString::number(mCurrentError);

	for(int i = 0; i < mErrorFunctions.size(); ++i)
	{
		error = mErrorFunctions[i]->add(mNetwork->outputs(), mTargetValues);
		//message += "\n\t\t" + mErrorFunctions[i]->name() + ": " + QString::number(error);
	}

	//LOG(message);
}

void ViTrainer::train()
{
	checkTargetValues();

	for(int i = 0; i < mErrorFunctions.size(); ++i)
	{
		mErrorFunctions[i]->clear();
	}
	if(mIterationLimit == 0)
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
		for(mCurrentIteration = 0; mCurrentIteration <= mIterationLimit; ++mCurrentIteration)
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
	ViElement element("trainer");
	element.addChild("name", name("Trainer"));
	element.addChild("iterationlimit", iterationLimit());
	element.addChild("errorlimit", errorLimit());
	element.addChild("learningrate", learningRate());

	ViElement errorFunctions("errorfunctions");
	for(int i = 0; i < mErrorFunctions.size(); ++i)
	{
		errorFunctions.addChild(mErrorFunctions[i]->exportData());
	}
	element.addChild(errorFunctions);

	return element;
}

bool ViTrainer::importData(ViElement element)
{
	if(element.name() != "trainer")
	{
		element = element.child("trainer");
		if(element.name() != "trainer")
		{
			return false;
		}
	}

	ViElement theName = element.child("name");
	if(theName.isNull())
	{
		return false;
	}
	if(theName.toString() != name("Trainer"))
	{
		return false;
	}

	ViElement limit = element.child("iterationlimit");
	if(limit.isNull())
	{
		LOG("The iteration limit could not be imported", QtCriticalMsg);
	}
	else
	{
		setIterationLimit(limit.toInt());
	}

	limit = element.child("errorlimit");
	if(limit.isNull())
	{
		LOG("The error limit could not be imported", QtCriticalMsg);
	}
	else
	{
		setErrorLimit(limit.toReal());
	}

	limit = element.child("learningrate");
	if(limit.isNull())
	{
		LOG("The learning could not be imported", QtCriticalMsg);
	}
	else
	{
		setLearningRate(limit.toReal());
	}

	addErrorFunction(ViErrorFunctionManager::create(element));

	return true;
}
