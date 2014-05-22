#ifndef VIFANN_H
#define VIFANN_H

#include <QList>
#include <doublefann.h> // FANN uses doubles
//#include <floatfann.h> // FANN uses floats

class ViFann
{

	public:

		enum Type
		{
			Standard,
			Sparse,
			Shortcut
		};

		enum Weights
		{
			Random,
			WidrowNguyen
		};

		enum Activation
		{
			Sigmoid,		// Symetric sigmoid (-1,1)
			Elliot,			// Symetric David Elliot (-1,1)
			Gaussian,		// Symetric Gaussian (-1,1)
			Linear,			// Symetric bounded linear (-1,1)
			Cosine,			// Symetric periodical cosinus [-1,1]
			Sine,			// Symetric periodical sinus [-1,1]
		};

	public:

		ViFann();
		ViFann(const ViFann &other);
		~ViFann();

		void clear();

		// Initializes the network structure
		// A list containing the number of neurons on each layer
		// Connection rate only for sparse networks (1 = fully connected, 0 = no connections)
		bool setStructure(const Type &type, const QList<int> &neurons, const qreal &connectionRate = 0.5);

		// Initialize the weights
		// Minimum and maximum will only be used if the initialization is random
		bool setWeights(Weights &initialization, const qreal &minimum = -0.1, const qreal &maximum = 0.1);

		// Set the activation function
		// Can be different for every layer
		bool setActivation(const Activation &activation); // Sets for all layers
		bool setActivation(const QList<Activation> &activations); // For each layer
		bool setActivation(const Activation &activation, const int &layer);


	private:

		fann *mNetwork;

};

#endif
