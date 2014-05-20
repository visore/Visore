#include <vifann.h>

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

void ViFann::create(const Type &type, const QList<int> &neurons, const qreal &connectionRate)
{
	clear();

	unsigned int layers = neurons.size();
	unsigned int layerNeurons[layers];
	for(int i = 0; i < layers; ++i) layerNeurons[i] = neurons[i];

	if(type == Standard) mNetwork = fann_create_standard_array(layers, layerNeurons);
	else if(type == Sparse) mNetwork = fann_create_sparse_array(connectionRate, layers, layerNeurons);
	else if(type == Shortcut) mNetwork = fann_create_shortcut_array(layers, layerNeurons);
}
