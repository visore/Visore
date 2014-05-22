#include <vifann.h>

// FANN default
#define MIN_RANDOM_WEIGHTS -0.1
#define MAX_RANDOM_WEIGHTS 0.1

ViFann::ViFann()
{
	mNetwork = NULL;
}

ViFann::ViFann(const ViFann &other)
{
	if(other.mNetwork == NULL) mNetwork = NULL;
	else mNetwork = fann_copy(other.mNetwork);
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
}

bool ViFann::setStructure(const Type &type, const QList<int> &neurons, const qreal &connectionRate)
{
	clear();

	unsigned int layers = neurons.size();
	unsigned int layerNeurons[layers];
	for(int i = 0; i < layers; ++i) layerNeurons[i] = neurons[i];

	if(type == Standard) mNetwork = fann_create_standard_array(layers, layerNeurons);
	else if(type == Sparse) mNetwork = fann_create_sparse_array(connectionRate, layers, layerNeurons);
	else if(type == Shortcut) mNetwork = fann_create_shortcut_array(layers, layerNeurons);
	else return false;
	return true;
}

bool ViFann::setWeights(Weights &initialization, const qreal &minimum, const qreal &maximum)
{
	if(mNetwork == NULL) return false;

	if(initialization == Random) fann_randomize_weights(mNetwork, MIN_RANDOM_WEIGHTS, MAX_RANDOM_WEIGHTS);
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
	for(int i = 1; i < layers; ++i)
	{
		if(!setActivation(activation, i)) return false;
	}
	return true;
}

bool ViFann::setActivation(const QList<Activation> &activations)
{
	if(mNetwork == NULL) return false;
	if(fann_get_num_layers(mNetwork) - 1 != activations.size()) return false; // -1 because of the input layer
	int layers = fann_get_num_layers(mNetwork);
	for(int i = 1; i < layers; ++i)
	{
		if(!setActivation(activations[i - 1], i)) return false;
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
