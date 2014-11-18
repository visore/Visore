#include <vifann.h>
#include <vilogger.h>
#include <QFile>

ViFannTrain::ViFannTrain(const int &dataCount, const int &inputs, const int &outputs)
{
	mData = fann_create_train(dataCount, inputs, outputs);
}

ViFannTrain::ViFannTrain(const ViFannTrain &other)
{
	mData = fann_create_train(other.mData->num_data, other.mData->num_input, other.mData->num_output);
	for(int i = 0; i < mData->num_data; ++i)
	{
		memcpy(mData->input[i], other.mData->input[i], sizeof(float) * mData->num_input);
		memcpy(mData->output[i], other.mData->output[i], sizeof(float) * mData->num_output);
	}
}

ViFannTrain::~ViFannTrain()
{
	fann_destroy_train(mData);
}

bool ViFannTrain::setInput(const int &index, const float *data)
{
	if(index < 0 || index >= mData->num_data)
	{
		LOG("Invalid index for the training input: " + QString::number(index) + " of " + QString::number(mData->num_data), QtCriticalMsg);
		exit(-1);
		return false;
	}
	memcpy(mData->input[index], data, sizeof(float) * mData->num_input);
	return true;
}

bool ViFannTrain::setOutput(const int &index, const float *data)
{
	if(index < 0 || index >= mData->num_data)
	{
		LOG("Invalid index for the training output: " + QString::number(index) + " of " + QString::number(mData->num_data), QtCriticalMsg);
		exit(-1);
		return false;
	}
	memcpy(mData->output[index], data, sizeof(float) * mData->num_output);
	return true;
}

fann_train_data* ViFannTrain::data()
{
	return mData;
}

bool ViFannTrain::isSame(const int &dataCount, const int &inputs, const int &outputs)
{
	return mData->num_data == dataCount && mData->num_input == inputs && mData->num_output == outputs;
}

ViFann::ViFann(const bool &useGpu)
{
	mNetwork = NULL;
	mTrain = NULL;
	mInput = NULL;
	mOutput = NULL;
	clear();
	enableGpu(useGpu);
}

ViFann::ViFann(const ViFann &other)
{
	#ifdef GPU
		LOG("The GPU version of FANN currently doesn't allow the copy of ANNs.", QtFatalMsg);
		exit(-1);
	#else
		mNetwork = NULL;
		mTrain = NULL;
		mInput = NULL;
		mOutput = NULL;
		clear();
		enableGpu(other.mGpu);

		if(other.mNetwork != NULL) mNetwork = fann_copy(other.mNetwork);
		if(other.mTrain != NULL) mTrain = new ViFannTrain(*other.mTrain);
		if(other.mInput != NULL) mInput = new float[other.mInputCount];
		if(other.mOutput != NULL) mOutput = new float[other.mOutputCount];

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

		// Training
		mTraining = other.mTraining;
		mAlgorithm = other.mAlgorithm;
		mTrainEpochs = other.mTrainEpochs;
		mTrainNeurons = other.mTrainNeurons;
		mTrainMse = other.mTrainMse;
		mTrainStagnationFraction = other.mTrainStagnationFraction;
		mTrainStagnationIterations = other.mTrainStagnationIterations;
	#endif
}

ViFann::~ViFann()
{
	clear();
}

void ViFann::enableGpu(const bool &enable)
{
	if(enable)
	{
		#ifdef GPU
			mGpu = true;
			fannTrainManyPointer = &ViFann::fannTrainManyGpu;
			fannTrainSinglePointer = &ViFann::fannTrainSingleGpu;
			LOG("FANN GPU processing enabled.");
		#else
			mGpu = false;
			fannTrainManyPointer = &ViFann::fannTrainManyCpu;
			fannTrainSinglePointer = &ViFann::fannTrainSingleCpu;
			LOG("Could not enable FANN GPU processing. No capable GPU found.", QtCriticalMsg);
		#endif
	}
	else
	{
		mGpu = false;
		fannTrainManyPointer = &ViFann::fannTrainManyCpu;
		fannTrainSinglePointer = &ViFann::fannTrainSingleCpu;
	}
}

void ViFann::clear()
{
	if(mNetwork != NULL)
	{
		fann_destroy(mNetwork);
		mNetwork = NULL;
	}

	// Other
	if(mInput != NULL)
	{
		delete [] mInput;
		mInput = NULL;
	}
	if(mOutput != NULL)
	{
		delete [] mOutput;
		mOutput = NULL;
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

	//Training
	if(mTrain != NULL)
	{
		delete mTrain;
		mTrain = NULL;
	}
	mTraining = Fixed;
	mAlgorithm = RProp;
	mTrainEpochs = 100;
	mTrainNeurons = 100;
	mTrainMse = 0;
	mTrainStagnationFraction = -1;
	mTrainStagnationIterations = -1;

}

bool ViFann::setStructure(const Type &type, const QList<int> &neurons, const qreal &connectionRate)
{
	#ifdef GPU
		if(type != Standard)
		{
			LOG("The GPU version of FANN currently doesn't support shortcut, sparse or cascade networks.", QtFatalMsg);
			exit(-1);
		}
	#endif
	clear();

	mType = type;
	mInputCount = neurons.first();
	mOutputCount = neurons.last();
	mNeurons.clear();
	for(mI = 0; mI < neurons.size(); ++mI)
	{
		if(neurons[mI] != 0) mNeurons.append(neurons[mI]);
	}

	if(mInput == NULL) delete [] mInput;
	if(mOutput == NULL) delete [] mOutput;
	mInput = new float[mInputCount];
	mOutput = new float[mOutputCount];

	unsigned int layers = mNeurons.size();
	unsigned int layerNeurons[layers];
	for(mI = 0; mI < layers; ++mI) layerNeurons[mI] = mNeurons[mI];

	if(type == Standard) mNetwork = fann_create_standard_array(layers, layerNeurons);
	#ifndef GPU
		else if(type == Sparse)
		{
			mNetwork = fann_create_sparse_array(connectionRate, layers, layerNeurons);
			mConnectionRate = connectionRate;
		}
		else if(type == Shortcut) mNetwork = fann_create_shortcut_array(layers, layerNeurons);
	#endif
	else return false;

	fann_set_train_stop_function(mNetwork, FANN_STOPFUNC_MSE);

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

void ViFann::setTraining(const Training &training, const Algorithm &algorithm)
{
	setTraining(training);
	setTraining(algorithm);
}

void ViFann::setTraining(const Training &training)
{
	mTraining = training;
}

void ViFann::setTraining(const Algorithm &algorithm)
{
	mAlgorithm = algorithm;
	if(mAlgorithm == Incremental) fann_set_training_algorithm(mNetwork, FANN_TRAIN_INCREMENTAL);
	else if(mAlgorithm == Batch) fann_set_training_algorithm(mNetwork, FANN_TRAIN_BATCH);
	else if(mAlgorithm == RProp) fann_set_training_algorithm(mNetwork, FANN_TRAIN_RPROP);
	else if(mAlgorithm == QuickProp) fann_set_training_algorithm(mNetwork, FANN_TRAIN_QUICKPROP);
}

void ViFann::setStopEpochs(const int &maxEpochs)
{
	mTrainEpochs = maxEpochs;
}

void ViFann::setStopNeurons(const int &maxNeurons)
{
	mTrainNeurons = maxNeurons;
}

void ViFann::setStopMse(const qreal &mse)
{
	mTrainMse = mse;
}

void ViFann::setStopStagnation(const qreal &fraction, const int &iterations)
{
	mTrainStagnationFraction = fraction;
	mTrainStagnationIterations = iterations;
}

ViFannTrain* ViFann::createTrain(const int &dataCount)
{
	if(mTrain != NULL) delete mTrain;
	mTrain = new ViFannTrain(dataCount, mInputCount, mOutputCount);
	return mTrain;
}

void ViFann::deleteTrain()
{
	delete mTrain;
	mTrain = NULL;
}

bool ViFann::setTrainInput(const int &index, const qreal *data)
{
	if(mTrain == NULL)
	{
		LOG("Training set not create yet. Call the createTrain function first.", QtCriticalMsg);
		return false;
	}
	toFloat(data, mInput, mInputCount);
	return mTrain->setInput(index, mInput);
}

bool ViFann::setTrainOutput(const int &index, const qreal *data)
{
	if(mTrain == NULL)
	{
		LOG("Training set not create yet. Call the createTrain function first.", QtCriticalMsg);
		return false;
	}
	toFloat(data, mOutput, mOutputCount);
	return mTrain->setOutput(index, mOutput);
}

bool ViFann::setTrainInput(const int &index, const float *data)
{
	if(mTrain == NULL)
	{
		LOG("Training set not create yet. Call the createTrain function first.", QtCriticalMsg);
		return false;
	}
	return mTrain->setInput(index, data);
}

bool ViFann::setTrainOutput(const int &index, const float *data)
{
	if(mTrain == NULL)
	{
		LOG("Training set not create yet. Call the createTrain function first.", QtCriticalMsg);
		return false;
	}
	return mTrain->setOutput(index, data);
}

bool ViFann::isValid()
{
	if(mType != Shortcut && mTraining == Cascade)
	{
		LOG("Cascade trainining can only be done on a shortcut network.");
		return false;
	}
	if(mTraining == Cascade && mNeurons.size() > 2)
	{
		LOG("Cascade trainining networks can not have hidden layers.");
		return false;
	}
	if((mAlgorithm == Incremental || mAlgorithm == Batch) && mTraining == Cascade)
	{
		LOG("Cascade trainining can only be done with the RProp and QuickProp training algorithms.");
		return false;
	}
	return true;
}

bool ViFann::saveToFile(const QString &path)
{
	return fann_save(mNetwork, path.toLatin1().data());
}

bool ViFann::loadFromFile(const QString &path)
{
	QFile file(path);
	if(!file.exists()) return false;

	clear();
	mNetwork = fann_create_from_file(path.toLatin1().data());
	if(mNetwork == NULL) return false;

	fann_nettype_enum type = fann_get_network_type(mNetwork);
	//TODO: get type

	int layerCount = fann_get_num_layers(mNetwork);
	unsigned int layers[layerCount];
	fann_get_layer_array(mNetwork, layers);
	for(int i = 0; i < layerCount; ++i)
	{
		mNeurons.append(layers[i]);
	}
	mInputCount = mNeurons.first();
	mOutputCount = mNeurons.last();

	if(mInput == NULL) delete [] mInput;
	if(mOutput == NULL) delete [] mOutput;
	mInput = new float[mInputCount];
	mOutput = new float[mOutputCount];

	mConnectionRate = fann_get_connection_rate(mNetwork);

	//TODO: get weights

	return true;
}

void ViFann::run(const qreal *input, qreal *output)
{
	toFloat(input, mInput, mInputCount);
	run(mInput, mOutput);
	toReal(mOutput, output, mOutputCount);
}

void ViFann::run(const qreal *input, qreal &output)
{
	toFloat(input, mInput, mInputCount);
	run(mInput, mSingle);
	output = mSingle;
}

void ViFann::run(const qreal *input, qreal *output, const int &iterations)
{
	toFloat(input, mInput, mInputCount);
	run(mInput, mOutput, iterations);
	toReal(mOutput, output, mOutputCount);
}

void ViFann::run(const float *input, float *output)
{
	// TODO: Should we delete result?
	float *result = fann_run(mNetwork, (float*) input);
	for(mI = 0; mI < mOutputCount; ++mI) output[mI] = result[mI];
	adjustSamples(output, mOutputCount);
}

void ViFann::run(const float *input, float &output)
{
	// TODO: Should we delete result?
	float *result = fann_run(mNetwork, (float*) input);
	output = result[0];
	adjustSample(output);
}

void ViFann::run(const float *input, float *output, const int &iterations)
{
	float newInput[mInputCount];
	memcpy(newInput, input, mInputCount * sizeof(float));
	int copySize = (mInputCount - 1) * sizeof(float);

	for(mI = 0; mI < iterations; ++mI)
	{
		float *result = fann_run(mNetwork, newInput);
		output[mI] = result[0];
		memcpy(newInput, newInput + 1, copySize);
		newInput[mInputCount - 1] = result[0];
	}
	adjustSamples(output, iterations);
}

void ViFann::train(const qreal *input, const qreal *desiredOutput)
{
	toFloat(input, mInput, mInputCount);
	toFloat(desiredOutput, mOutput, mOutputCount);
	train(mInput, mOutput);
}

void ViFann::train(const qreal *input, const qreal &desiredOutput)
{
	mSingle = desiredOutput;
	toFloat(input, mInput, mInputCount);
	train(mInput, mSingle);
}

void ViFann::train(const float *input, const float *desiredOutput)
{
	fann_train(mNetwork, (float*) input, (float*) desiredOutput);
}

void ViFann::train(const float *input, const float &desiredOutput)
{
	fann_train(mNetwork, (float*) input, (float*) &desiredOutput);
}

int ViFann::train(const bool &debug)
{
	if(mTrain == NULL)
	{
		LOG("The training dataset was not created. No training will be done.");
		return -1;
	}

	if(debug)
	{
		if(mTraining == Cascade)
		{
			#ifndef GPU
				if(mTrainStagnationIterations >= 0)
				{
					fann_set_cascade_output_change_fraction(mNetwork, mTrainStagnationFraction);
					fann_set_cascade_output_stagnation_epochs(mNetwork, mTrainStagnationIterations);
					fann_set_cascade_candidate_change_fraction(mNetwork, mTrainStagnationFraction);
					fann_set_cascade_candidate_stagnation_epochs(mNetwork, mTrainStagnationIterations);
				}
				fann_cascadetrain_on_data(mNetwork, mTrain->data(), mTrainNeurons, 1, mTrainMse);
			#endif
		}
		else if(mTrainStagnationIterations < 0) (this->*fannTrainManyPointer)(mNetwork, mTrain->data(), mTrainEpochs, 1, mTrainMse);
		else
		{
			int i, counter = 0;
			qreal previousMse, mse, difference;
			for(i = 1; i <= mTrainEpochs; ++i)
			{
				mse = (this->*fannTrainSinglePointer)(mNetwork, mTrain->data());
				difference = abs(previousMse - mse);
				previousMse = mse;
				LOG("Current epoch: " + QString::number(i) + "\t\tCurrent MSE: " + QString::number(mse, 'f', 10) + "\tMSE change fraction: " + QString::number(difference, 'f', 10) + " (desired: " + QString::number(mTrainStagnationFraction, 'f', 10) + ")");
				if(mse <= mTrainMse)
				{
					LOG("The minimum MSE was reached. Training was stopped.");
					break;
				}
				if(difference <= mTrainStagnationFraction)
				{
					if(counter >= mTrainStagnationIterations)
					{
						LOG("Training has stagnated with a MSE change fraction of " + QString::number(difference, 'f', 10) + ". Training was stopped.");
						break;
					}
					++counter;
				}
				else counter = 0;
			}
			delete mTrain;
			mTrain = NULL;
			return i - 1;
		}
	}
	else
	{
		if(mTraining == Cascade)
		{
			#ifndef GPU
				if(mTrainStagnationIterations >= 0)
				{
					fann_set_cascade_output_change_fraction(mNetwork, mTrainStagnationFraction);
					fann_set_cascade_output_stagnation_epochs(mNetwork, mTrainStagnationIterations);
					fann_set_cascade_candidate_change_fraction(mNetwork, mTrainStagnationFraction);
					fann_set_cascade_candidate_stagnation_epochs(mNetwork, mTrainStagnationIterations);
				}
				fann_cascadetrain_on_data(mNetwork, mTrain->data(), mTrainNeurons, 0, mTrainMse);
			#endif
		}
		else if(mTrainStagnationIterations < 0) (this->*fannTrainManyPointer)(mNetwork, mTrain->data(), mTrainEpochs, 0, mTrainMse);
		else
		{
			int i, counter = 0;
			qreal previousMse, mse, difference;
			for(i = 1; i <= mTrainEpochs; ++i)
			{
				mse = (this->*fannTrainSinglePointer)(mNetwork, mTrain->data());
				if(mse <= mTrainMse) break;
				difference = abs(previousMse - mse);
				previousMse = mse;
				if(difference <= mTrainStagnationFraction)
				{
					if(counter >= mTrainStagnationIterations) break;
					++counter;
				}
				else counter = 0;
			}
			delete mTrain;
			mTrain = NULL;
			return i - 1;
		}
	}

	delete mTrain;
	mTrain = NULL;
	return -1;
}

void ViFann::runTrain(const qreal *input, qreal *output, const qreal *desiredOutput)
{
	toFloat(input, mInput, mInputCount);
	toFloat(output, mOutput, mOutputCount);
	run(mInput, mOutput);
	toFloat(desiredOutput, mOutput, mOutputCount);
	train(mInput, mOutput);
}

void ViFann::runTrain(const qreal *input, qreal &output, const qreal &desiredOutput)
{
	toFloat(input, mInput, mInputCount);
	run(mInput, mSingle);
	output = mSingle;
	mSingle = desiredOutput;
	train(mInput, mSingle);
}

void ViFann::runTrain(const float *input, float *output, const float *desiredOutput)
{
	run(input, output);
	train(input, desiredOutput);
}

void ViFann::runTrain(const float *input, float &output, const float &desiredOutput)
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

qreal ViFann::mse()
{
	return fann_get_MSE(mNetwork);
}

fann* ViFann::network()
{
	return mNetwork;
}

void ViFann::fannTrainManyCpu(fann *network, fann_train_data *data, const unsigned int &maxEpochs, const unsigned int &epochsBetweenReports, const float &desiredError)
{
	fann_train_on_data(network, data, maxEpochs, epochsBetweenReports, desiredError);
}

float ViFann::fannTrainSingleCpu(fann *network, fann_train_data *data)
{
	return fann_train_epoch(network, data);
}

void ViFann::fannTrainManyGpu(fann *network, fann_train_data *data, const unsigned int &maxEpochs, const unsigned int &epochsBetweenReports, const float &desiredError)
{
	#ifdef GPU
		fann_train_on_data_cl(network, data, maxEpochs, epochsBetweenReports, desiredError);
	#endif
}

float ViFann::fannTrainSingleGpu(fann *network, fann_train_data *data)
{
	#ifdef GPU
		return fann_train_epoch_cl(network, data);
	#endif
}

inline void ViFann::toFloat(const qreal *input, float *output, const int &size)
{
	for(int i = 0; i < size; ++i) output[i] = input[i];
}

inline void ViFann::toReal(const float *input, qreal *output, const int &size)
{
	for(int i = 0; i < size; ++i) output[i] = input[i];
}

inline void ViFann::adjustSamples(float *samples, const int &size)
{
	for(mI = 0; mI < size; ++mI)
	{
		if(samples[mI] > 1) samples[mI] = 1;
		else if(samples[mI] < -1) samples[mI] = -1;
	}
}

inline void ViFann::adjustSample(float &sample)
{
	if(sample > 1) sample = 1;
	else if(sample < -1) sample = -1;
}
