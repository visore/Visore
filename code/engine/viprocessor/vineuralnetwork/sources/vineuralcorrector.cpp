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
	/*mFactory.setActivationFunction(ViActivationFunctionManager::create("ViSigmoidActivationFunction"));
	mFactory.addLayer(3);
	mFactory.addLayer(2);
	mFactory.addLayer(1);
	ViNeuralNetwork *network = mFactory.create();
	network->setInputs({1,0,1});



	ViTrainer *trainer = ViTrainerManager::createDefault();
	//ViWeightInitializer *weightInitializer = ViWeightInitializerManager::createDefault();
	ViFixedWeightInitializer *weightInitializer = (ViFixedWeightInitializer*) ViWeightInitializerManager::create("Fixed");

	weightInitializer->setValues({0.2,-0.3,0.4,0.1,-0.5,0.2,-0.3,-0.2,-0.4,0.2,0.1});
	weightInitializer->initialize(network, trainer->learningRate());

network->run();

cout<<"hidden 1: "<<network->at(1)->at(0)->value()<<endl;
cout<<"hidden 2: "<<network->at(1)->at(1)->value()<<endl;
cout<<"output: "<<network->at(2)->at(0)->value()<<endl;

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
	}*/
}

void ViNeuralCorrector::execute()
{

}

void ViNeuralCorrector::finalize()
{

}
