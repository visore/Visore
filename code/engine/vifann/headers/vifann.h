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

		// In [0,1]
		// Default in FANN: 0.7 (typical at 0.3 with momentum at 0.9)
		// Training parameter that controls the size of weight and bias changes during learning
		void setLearningRate(const qreal &rate);

		// In [0,1]
		// Default in FANN: 0  (typical at 0.9 with learning rate at 0.3)
		// Momentum simply adds a fraction m of the previous weight update to the current one.
		// The momentum parameter is used to prevent the system from converging to a local minimum or saddle point.
		// A high momentum parameter can also help to increase the speed of convergence of the system.
		// However, setting the momentum parameter too high can create a risk of overshooting the minimum, which can cause the system to become unstable.
		// A momentum coefficient that is too low cannot reliably avoid local minima, and can also slow down the training of the system.
		void setLearningMomentum(const qreal &momentum);

		// Run the input through the network and generate the output
		// Does not train the newtork
		// Input array should at least have the same size than the number of input neurons, same with output array
		void run(qreal *input, qreal *output);
		void run(qreal *input, qreal &output); // For a single output neuron

		// Train the network with a single interation, using the input and desired output
		// Input array should at least have the same size than the number of input neurons, same with output array
		void train(qreal *input, qreal *desiredOutput);
		void train(qreal *input, const qreal &desiredOutput);

	private:

		fann *mNetwork;
		int mI;
		int mInputCount;
		int mOutputCount;
		qreal *mSingleArray;

};

#endif
