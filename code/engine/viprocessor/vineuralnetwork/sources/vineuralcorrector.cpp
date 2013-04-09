#include <vineuralcorrector.h>

#include <viactivationfunctionmanager.h>
#include <viweightinitializermanager.h>
#include <vitrainermanager.h>

#include <vifixedweightinitializer.h>
#include <visigmoidactivationfunction.h>

ViNeuralCorrector::ViNeuralCorrector()
	: ViProcessor()
{
	mNetwork = NULL;
}

ViNeuralCorrector::~ViNeuralCorrector()
{
	if(mNetwork != NULL)
	{
		delete mNetwork;
		mNetwork = NULL;
	}
}

void ViNeuralCorrector::initialize()
{
	mFactory.setActivationFunction(ViActivationFunctionManager::create("ViSigmoidActivationFunction"));
	mFactory.addLayer(2);
	mFactory.addLayer(2);
	mFactory.addLayer(1);
	ViNeuralNetwork *network = mFactory.create();
	network->setInputs({0.35, 0.9});

ViSigmoidActivationFunction ff;
cout<<"ppppppppppp: "<< ff.calculate(0.755)<<endl;

	ViTrainer *trainer = ViTrainerManager::createDefault();
	//ViWeightInitializer *weightInitializer = ViWeightInitializerManager::createDefault();
	ViFixedWeightInitializer *weightInitializer = (ViFixedWeightInitializer*) ViWeightInitializerManager::create("Fixed");

	weightInitializer->setValues({0.1, 0.4, 0.8, 0.6, 0.3, 0.9});
	weightInitializer->initialize(network, trainer->learningRate());

	int c = 0;
	for(int i = 0; i < network->size()-1; ++i)
	{
		for(int j = 0; j < network->at(i)->size(); ++j)
		{
			for(int k = 0; k < network->at(i)->at(j)->outputSize(); ++k)
			{
				cout << "w" << ++c << ": " << network->at(i)->at(j)->outputAt(k)->weight() << endl;
			}
		}
	}

	trainer->trainSingle(network);
	c = 0;
	for(int i = 0; i < network->size()-1; ++i)
	{
		for(int j = 0; j < network->at(i)->size(); ++j)
		{
			for(int k = 0; k < network->at(i)->at(j)->outputSize(); ++k)
			{
				cout << "w" << ++c << ": " << network->at(i)->at(j)->outputAt(k)->weight() << endl;
			}
		}
	}
}

void ViNeuralCorrector::execute()
{

}

void ViNeuralCorrector::finalize()
{

}
