#include <vineuralcorrector.h>
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
	//mFactory.setActivationFunction(ViSigmoidActivationFunction::instance());
}

void ViNeuralCorrector::execute()
{

}

void ViNeuralCorrector::finalize()
{

}
