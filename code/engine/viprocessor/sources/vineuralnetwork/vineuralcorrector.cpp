#include <vineuralcorrector.h>
#include <visigmoidactivationfunction.h>
#include <virandomweightinitializer.h>

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
	mFactory.setActivationFunction(new ViSigmoidActivationFunction());
	//mFactory.setWeight(new ViRandomWeightInitializer());
	mFactory.addLayer(5);
	mFactory.addLayer(3, 1);
	mFactory.addLayer(1);
	ViNeuralNetwork *network = mFactory.create();
	network->exportFile(QString("/home/visore/NN.xml"));

	ViNeuralNetworkFactory f;
	ViNeuralNetwork *c = mFactory.create(network->exportData());
}

void ViNeuralCorrector::execute()
{

}

void ViNeuralCorrector::finalize()
{

}
