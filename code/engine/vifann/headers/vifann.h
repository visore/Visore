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
			NoType,
			Standard,
			Sparse,
			Shortcut
		};

		enum Weights
		{
			NoWeights,
			Random,
			WidrowNguyen
		};

		enum Activation
		{
			NoActivation,
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

		// Initializes the network inputs
		bool setInputCount(const int &count);

		// Initializes the network outputs
		bool setOutputCount(const int &count);

		// Initialize the weights
		// Minimum and maximum will only be used if the initialization is random
		bool setWeights(const Weights &initialization, const qreal &minimum = -0.1, const qreal &maximum = 0.1);

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
		// Default in FANN: 0 (typical at 0.9 with learning rate at 0.3)
		// Momentum simply adds a fraction m of the previous weight update to the current one.
		// The momentum parameter is used to prevent the system from converging to a local minimum or saddle point.
		// A high momentum parameter can also help to increase the speed of convergence of the system.
		// However, setting the momentum parameter too high can create a risk of overshooting the minimum, which can cause the system to become unstable.
		// A momentum coefficient that is too low cannot reliably avoid local minima, and can also slow down the training of the system.
		void setLearningMomentum(const qreal &momentum);

		// Run the input through the network and generate the output
		// Does not train the newtork
		// Input array should at least have the same size than the number of input neurons, same with output array
		void run(const qreal *input, qreal *output);
		void run(const qreal *input, qreal &output); // For a single output neuron
		void run(const qreal *input, qreal *output, const int &interations); // Generates 1 output at a time, feeds the output into the input and generates the next output (for interations outputs)

		// Train the network with a single interation, using the input and desired output
		// Input array should at least have the same size than the number of input neurons, same with output array
		void train(const qreal *input, const qreal *desiredOutput);
		void train(const qreal *input, const qreal &desiredOutput);

		// First run the input, and then train it
		void runTrain(const qreal *input, qreal *output, const qreal *desiredOutput);
		void runTrain(const qreal *input, qreal &output, const qreal &desiredOutput);

		// Getters

		Type type();
		int inputCount();
		int outputCount();
		int layerCount();
		int neuronCount(const int &layer);
		QList<int> neurons();
		qreal connectionRate();

		Weights weights();
		qreal weightsMinimum();
		qreal weightsMaximum();

	protected:

		inline void adjustSamples(qreal *samples, const int &size);
		inline void adjustSample(qreal &sample);

	private:

		// Other
		int mI;

		// Network
		fann *mNetwork;

		//Structure
		Type mType;
		int mInputCount;
		int mOutputCount;
		QList<int> mNeurons;
		qreal mConnectionRate;

		// Weights
		Weights mWeights;
		qreal mWeightsMinimum;
		qreal mWeightsMaximum;

};

#endif
