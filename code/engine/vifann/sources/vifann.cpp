#include <vifann.h>

ViFann::ViFann()
{
	mNetwork = NULL;
	clear();
}

ViFann::ViFann(const ViFann &other)
{
	mNetwork = NULL;
	clear();

	if(other.mNetwork == NULL) mNetwork = NULL;
	else mNetwork = fann_copy(other.mNetwork);

	// Structure
	mType = other.mType;
	mInputCount = other.mInputCount;
	mOutputCount = other.mOutputCount;
	mNeurons = other.mNeurons;
	mConnectionRate = other.mConnectionRate;

	// Weights
	mWeights = other.mWeights;
	mWeightsMinimum = other.mWeightsMinimum;
	mWeightsMaximum = other.mWeightsMaximum;
}

ViFann::~ViFann()
{
	clear();
}

void ViFann::clear()
{
	if(mNetwork != NULL)
	{
		fann_destroy(mNetwork);
		mNetwork = NULL;
	}

	// Structure
	mType = NoType;
	mInputCount = 0;
	mOutputCount = 0;
	mNeurons.clear();
	mConnectionRate = 0;

	// Weights
	mWeights = NoWeights;
	mWeightsMinimum = 0;
	mWeightsMaximum = 0;
}

bool ViFann::setStructure(const Type &type, const QList<int> &neurons, const qreal &connectionRate)
{
	clear();

	unsigned int layers = neurons.size();
	unsigned int layerNeurons[layers];
	for(mI = 0; mI < layers; ++mI) layerNeurons[mI] = neurons[mI];

	mType = type;
	mInputCount = neurons.first();
	mOutputCount = neurons.last();
	mNeurons = neurons;

	if(type == Standard) mNetwork = fann_create_standard_array(layers, layerNeurons);
	else if(type == Sparse)
	{
		mNetwork = fann_create_sparse_array(connectionRate, layers, layerNeurons);
		mConnectionRate = connectionRate;
	}
	else if(type == Shortcut) mNetwork = fann_create_shortcut_array(layers, layerNeurons);
	else return false;



	//fann_set_training_algorithm(mNetwork,FANN_TRAIN_INCREMENTAL);

	//fann_set_train_error_function(mNetwork,FANN_ERRORFUNC_LINEAR);


	return true;
}

bool ViFann::setInputCount(const int &count)
{
	if(mNeurons.isEmpty()) return false;
	QList<int> neurons = mNeurons;
	neurons.first() = count;
	return setStructure(mType, neurons, mConnectionRate);
}

bool ViFann::setOutputCount(const int &count)
{
	if(mNeurons.isEmpty()) return false;
	QList<int> neurons = mNeurons;
	neurons.last() = count;
	return setStructure(mType, neurons, mConnectionRate);
}

bool ViFann::setWeights(const Weights &initialization, const qreal &minimum, const qreal &maximum)
{
	if(mNetwork == NULL) return false;
	mWeights = initialization;

	if(initialization == Random)
	{
		fann_randomize_weights(mNetwork, minimum, maximum);
		mWeightsMinimum = minimum;
		mWeightsMaximum = maximum;
	}
	else if(initialization == WidrowNguyen)
	{
		// Create fake training set so that FANN can determine the min and max values
		fann_train_data *data = fann_create_train(1, 2, 1);
		data->input[0][0] = 1;
		data->input[0][1] = -1;
		fann_init_weights(mNetwork, data);
		fann_destroy_train(data);
	}

	return true;
}

bool ViFann::setActivation(const Activation &activation)
{
	if(mNetwork == NULL) return false;
	int layers = fann_get_num_layers(mNetwork);
	for(mI = 1; mI < layers; ++mI)
	{
		if(!setActivation(activation, mI)) return false;
	}
	return true;
}

bool ViFann::setActivation(const QList<Activation> &activations)
{
	if(mNetwork == NULL) return false;
	if(fann_get_num_layers(mNetwork) - 1 != activations.size()) return false; // -1 because of the input layer
	int layers = fann_get_num_layers(mNetwork);
	for(mI = 1; mI < layers; ++mI)
	{
		if(!setActivation(activations[mI - 1], mI)) return false;
	}
	return true;
}

bool ViFann::setActivation(const Activation &activation, const int &layer)
{
	if(mNetwork == NULL) return false;

	fann_activationfunc_enum function;
	if(activation == Sigmoid) function = FANN_SIGMOID_SYMMETRIC;
	else if(activation == Elliot) function = FANN_ELLIOT_SYMMETRIC;
	else if(activation == Gaussian) function = FANN_GAUSSIAN_SYMMETRIC;
	else if(activation == Linear) function = FANN_LINEAR_PIECE_SYMMETRIC;
	else if(activation == Cosine) function = FANN_COS_SYMMETRIC;
	else if(activation == Sine) function = FANN_SIN_SYMMETRIC;
	else return false;

	fann_set_activation_function_layer(mNetwork, function, layer);
	return true;
}

void ViFann::setLearningRate(const qreal &rate)
{
	fann_set_learning_rate(mNetwork, rate);
}

void ViFann::setLearningMomentum(const qreal &momentum)
{
	fann_set_learning_momentum(mNetwork, momentum);
}

void ViFann::run(const qreal *input, qreal *output)
{
	// TODO: Should we delete result?
	qreal *result = FANN_API fann_run(mNetwork, (qreal*) input);
	for(mI = 0; mI < mOutputCount; ++mI) output[mI] = result[mI];
	adjustSamples(output, mOutputCount);
}

void ViFann::run(const qreal *input, qreal &output)
{
	// TODO: Should we delete result?
	qreal *result = FANN_API fann_run(mNetwork, (qreal*) input);
	output = result[0];
	adjustSample(output);
}

void ViFann::run(const qreal *input, qreal *output, const int &interations)
{
	qreal newInput[mInputCount];
	memcpy(newInput, input, mInputCount * sizeof(qreal));
	qreal copySize = (mInputCount - 1) * sizeof(qreal);

	for(mI = 0; mI < interations; ++mI)
	{
		qreal *result = FANN_API fann_run(mNetwork, newInput);
		output[mI] = result[0];
		memcpy(newInput, newInput + 1, copySize);
		newInput[mInputCount - 1] = result[0];
	}
	adjustSamples(output, interations);
}

void ViFann::train(const qreal *input, const qreal *desiredOutput)
{
	fann_train(mNetwork, (qreal*) input, (qreal*) desiredOutput);
}

void ViFann::train(const qreal *input, const qreal &desiredOutput)
{
	fann_train(mNetwork, (qreal*) input, (qreal*) &desiredOutput);
}

void ViFann::runTrain(const qreal *input, qreal *output, const qreal *desiredOutput)
{
	run(input, output);
	train(input, desiredOutput);
}

void ViFann::runTrain(const qreal *input, qreal &output, const qreal &desiredOutput)
{
	run(input, output);
	train(input, desiredOutput);
}

ViFann::Type ViFann::type()
{
	return mType;
}

int ViFann::inputCount()
{
	return mInputCount;
}

int ViFann::outputCount()
{
	return mOutputCount;
}

int ViFann::layerCount()
{
	return mNeurons.size();
}

int ViFann::neuronCount(const int &layer)
{
	if(layer < 0 || layer >= mNeurons.size()) return 0;
	return mNeurons[layer];
}

QList<int> ViFann::neurons()
{
	return mNeurons;
}

qreal ViFann::connectionRate()
{
	return mConnectionRate;
}

ViFann::Weights ViFann::weights()
{
	return mWeights;
}

qreal ViFann::weightsMinimum()
{
	return mWeightsMinimum;
}

qreal ViFann::weightsMaximum()
{
	return mWeightsMaximum;
}

inline void ViFann::adjustSamples(qreal *samples, const int &size)
{
	for(mI = 0; mI < size; ++mI)
	{
		if(samples[mI] > 1) samples[mI] = 1;
		else if(samples[mI] < -1) samples[mI] = -1;
	}
}

inline void ViFann::adjustSample(qreal &sample)
{
	if(sample > 1) sample = 1;
	else if(sample < -1) sample = -1;
}
